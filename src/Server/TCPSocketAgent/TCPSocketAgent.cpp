//
// Created by 2ToThe10th on 03.04.2020.
//

#include "TCPSocketAgent.h"
#include "../../EpollOneReturn.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Server::TCPSocketAgent {

TCPSocketAgent::TCPSocketAgent(ServerMap &main_map) : main_map_(main_map) {}

void TCPSocketAgent::Initialize(size_t port) {

  accept_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (inet_aton("0.0.0.0", &addr.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonException();
  }

  if (bind(accept_socket_, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  if (listen(accept_socket_, 128) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  tcp_accept_thread_ = std::thread([this] { this->AcceptLoop(); });

  tcp_write_thread_ = std::thread([this] { this->CheckHashAndWriteLoop(); });
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
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *>(&height),
                            sizeof(height));
  TCPSocketHelper::WriteAll(socket, reinterpret_cast<const char *>(&width),
                            sizeof(width));
  TCPSocketHelper::WriteAll(
      socket,
      reinterpret_cast<const char *>(main_map_.GetImage().getPixelsPtr()),
      4 * height * width);
}

void TCPSocketAgent::SetAndSendPlayerId(int client_socket) {
  unsigned client_id = main_map_.AddPlayer();

  TCPSocketHelper::WriteAll(client_socket,
                            reinterpret_cast<const char *>(&client_id),
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

std::vector<uint64_t>
TCPSocketAgent::HashVectorFromBuffer(TCPSocketHelper::ConstBuffer &buffer) {
  std::vector<uint64_t> client_hash_vector;
  unsigned cur_position_in_client_hash = 0;
  while (cur_position_in_client_hash <= buffer.GetSize() - sizeof(uint64_t)) {
    uint64_t hash;
    memcpy(&hash, buffer.GetBuffer() + cur_position_in_client_hash,
           sizeof(hash));
    client_hash_vector.emplace_back(hash);
    cur_position_in_client_hash += sizeof(uint64_t);
  }
  return std::move(client_hash_vector);
}

void TCPSocketAgent::CheckHashAndWriteLoop() {

  while (is_work_) {
    int client_socket;
    if (client_epoll_.WaitWithFd(kEpollTimeoutMillisecond, client_socket)) {
      try {
        auto client_hash =
            TCPSocketHelper::ConstBuffer::ReadFrom(client_socket);
        if (client_hash.GetBuffer() == nullptr) {
          throw std::runtime_error("Player left game");
        }
        auto current_hash = main_map_.GetHash();
        uint64_t client_hash_of_all = *(uint64_t *)client_hash.GetBuffer();
        if (client_hash.GetSize() / sizeof(uint64_t) != current_hash.size()) {
          throw std::range_error("Client hash size != Current hash size");
        }
        if (client_hash_of_all != current_hash[0]) {
          auto client_hash_vector = HashVectorFromBuffer(client_hash);
          auto buffer = main_map_.GetDifference(client_hash_vector);
          buffer.WriteTo(client_socket);
        }
      } catch (...) { // ignore
        close(client_socket);
      }
    }
  }
}

} // namespace Server::TCPSocketAgent