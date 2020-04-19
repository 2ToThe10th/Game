//
// Created by 2ToThe10th on 03.04.2020.
//

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "UDPSocketAgent.h"
#include "../../UserAction.h"
#include "../../Marshaling.h"
#include "../../EpollOneReturn.h"


namespace Server::UDPSocketAgent {

UDPSocketAgent::UDPSocketAgent(ServerMap &main_map) : main_map_(main_map) {

}

void UDPSocketAgent::Initialize(size_t port, int event_fd) {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (inet_aton("127.0.0.1", &addr.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonException();
  }

  if (bind(socket_, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  udp_read_thread = std::thread([this] {
    this->ReadLoop();
  });

  udp_write_thread = std::thread([this, event_fd] {
    this->WriteLoop(0);
  });
}

void UDPSocketAgent::ReadLoop() {
  EpollOneReturn epoll;

  epoll.Add(socket_);

  char buffer[kMaxBufferSize + sizeof(unsigned)];

  while (is_work_) {

    if (epoll.Wait(kTimeoutEpollRead)) {
      struct sockaddr_in addr{};
      socklen_t addr_size = sizeof(addr);
      ssize_t was_received =
          recvfrom(socket_, buffer, kMaxBufferSize + sizeof(unsigned), 0,
                   reinterpret_cast<sockaddr *>(&addr), &addr_size);
      unsigned player_id = *(unsigned *) buffer;

      if (was_received < sizeof(player_id)) {
        throw std::system_error(errno, std::generic_category());
      }

      if (client_addresses_.size() <= player_id) {
        client_addresses_.resize(player_id + 1);
      }

      if (!client_addresses_[player_id].IsEqual(&addr, addr_size)) {
        client_addresses_[player_id].SetFrom(&addr, addr_size);
      }

      main_map_.udp_to_physics_queue_.PushBack(
          UserUpdate(player_id, Marshaling::FromUDPMessageToUserAction(
              buffer + sizeof(player_id),
              was_received - sizeof(player_id))));
    }
  }
}

void UDPSocketAgent::Close() {
  is_work_ = false;
  if (udp_read_thread.joinable()) {
    udp_read_thread.join();
  }
  if (udp_write_thread.joinable()) {
    udp_write_thread.join();
  }
  close(socket_);
}

void UDPSocketAgent::WriteLoop(int event_fd) {
  EpollOneReturn epoll;

  epoll.Add(event_fd);

  while (is_work_) {
    if (epoll.Wait(kTimeoutWrite)) {
      uint64_t event_value;
      read(event_fd, &event_value, sizeof(event_value));
    }
    auto to_send = main_map_.SynchronizeAndPrepareSendString();
    for (auto & client_address : client_addresses_) {
      if (!client_address.IsEmpty()) {
        sendto(socket_,
               to_send.GetBuffer(),
               to_send.GetSize(),
               0,
               reinterpret_cast<const sockaddr *>(&client_address.addr_),
               client_address.size_);
      }
    }
  }
}

UDPSocketAgent::ClientAddr::ClientAddr() = default;

bool UDPSocketAgent::ClientAddr::IsEmpty() {
  return size_ == 0;
}

bool UDPSocketAgent::ClientAddr::IsEqual(sockaddr_in *addr, socklen_t socklen) {
  if (IsEmpty()) {
    return false;
  }
  if (size_ != socklen) {
    return false;
  }
  return memcmp(&addr_, addr, socklen) == 0;
}

void UDPSocketAgent::ClientAddr::SetFrom(sockaddr_in *addr, socklen_t socklen) {
  memcpy(&addr_, addr, socklen);
  size_ = socklen;
}

}