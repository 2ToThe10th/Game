//
// Created by 2ToThe10th on 03.04.2020.
//

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TCPSocketAgent.h"
#include "../../EpollOneReturn.h"


namespace Server::TCPSocketAgent {

TCPSocketAgent::TCPSocketAgent(ServerMap &main_map) : main_map_(main_map) {

}

void TCPSocketAgent::Initialize(size_t port) {

  accept_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(port),};

  if (inet_aton("0.0.0.0", &addr.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonException();
  }

  if (bind(accept_socket_, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  if (listen(accept_socket_, 128) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  tcp_accept_thread_ = std::thread([this] {
    this->AcceptLoop();
  });

  tcp_write_thread_ = std::thread([this] {
    this->CheckHashAndWriteLoop();
  });
}

void TCPSocketAgent::AcceptLoop() {

  auto epoll = EpollOneReturn();

  TCPSocketHelper::MakeNonblock(accept_socket_);
  epoll.Add(accept_socket_);

  while (is_work_) {

    if (epoll.Wait(kEpollTimeoutMillisecond)) {

      int client_socket = accept(accept_socket_, nullptr, nullptr);

      if (client_socket < 0) {
        if (TCPSocketHelper::WouldBlock()) {
          break;
        } else {
          throw std::system_error(errno, std::generic_category());
        }
      }

      SendImage(client_socket);
      SetAndSendPlayerId(client_socket);

    }
  }
}

void TCPSocketAgent::SendImage(int socket) {
  unsigned height = main_map_.GetImage().getSize().y;
  unsigned width = main_map_.GetImage().getSize().x;
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *> (&height), sizeof(height));
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *> (&width), sizeof(width));
  TCPSocketHelper::WriteAll(socket,
                            reinterpret_cast<const char *> (main_map_.GetImage().getPixelsPtr()),
                            4 * height * width);
}

void TCPSocketAgent::SetAndSendPlayerId(int client_socket) {
  unsigned client_id = main_map_.AddPlayer();

  TCPSocketHelper::WriteAll(client_socket,
                            reinterpret_cast<const char *> (&client_id),
                            sizeof(client_id));

  client_epoll_.Add(client_socket);
}

void TCPSocketAgent::Close() {
  is_work_ = false;
  if (tcp_accept_thread_.joinable()) {
    tcp_accept_thread_.join();
  }
  if (tcp_write_thread_.joinable()) {
    tcp_write_thread_.join();
  }
  close(accept_socket_);
}

void TCPSocketAgent::CheckHashAndWriteLoop() {

  while (is_work_) {
    int client_socket;
    if (client_epoll_.WaitWithFd(kEpollTimeoutMillisecond, client_socket)) {
      uint64_t client_hash;
      try {
        TCPSocketHelper::ReadAll(client_socket,
                                 reinterpret_cast<char *>(&client_hash),
                                 sizeof(client_hash));
      } catch (std::system_error &system_error) {
        close(client_socket);
      }
      auto current_hash = main_map_.GetHash();
      if (client_hash != current_hash) {
        auto buffer = main_map_.GetCurrentInfo();
        try {
          buffer.WriteTo(client_socket);
        } catch (std::system_error &system_error) {
          close(client_socket);
        }
      }
    }
  }
}

}