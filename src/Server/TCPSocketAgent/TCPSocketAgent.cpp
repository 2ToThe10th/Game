//
// Created by 2ToThe10th on 03.04.2020.
//

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "TCPSocketAgent.h"
#include "../../TCPSocketHelper.h"


namespace Server::TCPSocketAgent {

TCPSocketAgent::TCPSocketAgent(ServerMap &main_map) : main_map_(main_map) {

}

void TCPSocketAgent::Initialize(size_t port) {
  sigset_t set;
  sigfillset(&set);
  sigprocmask(SIG_SETMASK, &set, nullptr);

  accept_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (inet_aton("127.0.0.1", &addr.sin_addr) != 1) {
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
    this->WriteLoop();
  });
}

void TCPSocketAgent::AcceptLoop() {

  auto epoll = TCPSocketHelper::EpollOneReturn();

  TCPSocketHelper::MakeNonblock(accept_socket_);
  epoll.Add(accept_socket_, nullptr);

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
  int height = main_map_.GetImage().getSize().y;
  int width = main_map_.GetImage().getSize().x;
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *> (&height), sizeof(int));
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *> (&width), sizeof(int));
  TCPSocketHelper::WriteAll(socket,
                            reinterpret_cast<const char *> (main_map_.GetImage().getPixelsPtr()),
                            4 * height * width);
}

void TCPSocketAgent::SetAndSendPlayerId(int client_socket) {
  size_t client_id = main_map_.AddPlayer();

  if (client_id >= client_sockets_.size()) {
    client_sockets_.resize(client_id + 1, -1);
  }

  TCPSocketHelper::WriteAll(client_socket,
                            reinterpret_cast<const char *> (&client_id),
                            sizeof(client_id));

  client_sockets_[client_id] = client_socket;
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

void TCPSocketAgent::WriteLoop() {
  while (is_work_) {
    auto buffer = main_map_.GetCurrentInfo();
    size_t player_id = 0;
    for (auto& socket: client_sockets_) {
      if (socket > -1) {
        try {
          buffer.WriteTo(socket);
        } catch (std::system_error &system_error) {
          close(socket);
          socket = -1;
          main_map_.DeletePlayer(player_id);
        }
      }
      ++player_id;
    }
    std::this_thread::sleep_for(kWriteAllPeriod);
  }
}

}