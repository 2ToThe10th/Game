//
// Created by 2ToThe10th on 03.04.2020.
//

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "UDPSocketAgent.h"
#include "../../UserAction.h"
#include "../../Marshaling.h"
#include "../../EpollOneReturn.h"


namespace Server::UDPSocketAgent {

UDPSocketAgent::UDPSocketAgent(ServerMap &main_map) : main_map_(main_map) {

}

void UDPSocketAgent::Initialize(size_t port) {
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
}

void StupidPhysicalAgent(unsigned player_id, UserAction user_action, ServerMap &server_map) {
  Location location;
  switch (user_action.GetAngle()) {
    case Angle::Up:
      location = server_map.GetPlayerLocation(player_id);
      server_map.SetPlayerLocation(player_id, Location(location.GetX(), location.GetY() - 0.01));
      break;
    default:
      break;
  }
}

void UDPSocketAgent::ReadLoop() {
  EpollOneReturn epoll;

  epoll.Add(socket_, nullptr);

  char buffer[kMaxBufferSize + sizeof(unsigned)];
  while (is_work_) {

    if (epoll.Wait(kTimeoutMillisecond)) {
      int was_received =
          recvfrom(socket_, buffer, kMaxBufferSize + sizeof(unsigned), 0, nullptr, nullptr);
      unsigned player_id = *(int *) buffer;

      buffer[was_received] = '\0';

      if (was_received < sizeof(player_id)) {
        throw std::system_error(errno, std::generic_category());
      }

      StupidPhysicalAgent(player_id,
                          Marshaling::FromUDPMessageToUserAction(buffer + sizeof(player_id),
                                                                 was_received - sizeof(player_id)),
                          main_map_);
    }
  }
}

void UDPSocketAgent::Close() {
  is_work_ = false;
  if (udp_read_thread.joinable()) {
    udp_read_thread.join();
  }
  close(socket_);
}

bool UDPSocketAgent::ClientAddr::IsEmpty() {
  return addr_ == nullptr;
}
}