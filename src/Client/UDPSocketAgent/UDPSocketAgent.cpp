//
// Created by 2ToThe10th on 02.04.2020.
//

#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "UDPSocketAgent.h"
#include "../../UDPSocketHelper.h"
#include "../../EpollOneReturn.h"


namespace Client::UDPSocketAgent {

UDPSocketAgent::UDPSocketAgent(ClientMap &main_map) : main_map_(main_map) {

}

void UDPSocketAgent::Initialize(const std::string &host, const size_t port, unsigned player_id) {
  player_id_ = player_id;

  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  server_addr_.sin_family = AF_INET;
  server_addr_.sin_port = htons(port);

  if (inet_aton(host.c_str(), &server_addr_.sin_addr) != 1) {
    throw std::system_error(errno, std::generic_category());
  }

  udp_receive_from_server_loop_ = std::thread([this] {
    this->ReceiveFromServer();
  });
}

void UDPSocketAgent::WriteToServer(const std::string &message) {
  size_t real_size = message.size() + sizeof(player_id_);
  char real_message[real_size];
  memcpy(real_message, &player_id_, sizeof(player_id_));
  memcpy(real_message + sizeof(player_id_), message.c_str(), message.size());
  UDPSocketHelper::WriteAll(socket_, server_addr_, real_message, real_size);
}

void UDPSocketAgent::Close() {
  is_work_ = false;
  if (udp_receive_from_server_loop_.joinable()) {
    udp_receive_from_server_loop_.join();
  }
  close(socket_);
}

void UDPSocketAgent::ReceiveFromServer() {
  EpollOneReturn epoll;

  epoll.Add(socket_);

  char buffer[kMaxBufferSize];

  while (is_work_) {

    if (epoll.Wait(kTimeoutMillisecond)) {
      ssize_t was_received =
          recvfrom(socket_, buffer, kMaxBufferSize, 0,
                   nullptr, nullptr);

      if (was_received < 0) {
        throw std::system_error(errno, std::generic_category());
      }

      unsigned player_id = -1;
      char *current_position_in_buffer = buffer;

      while (was_received - (current_position_in_buffer - buffer) >= sizeof(player_id) + Player::LengthToSend()) {
        player_id = *(unsigned *) current_position_in_buffer;
        current_position_in_buffer += sizeof(player_id);

        main_map_.UpdatePlayer(player_id, current_position_in_buffer);
        current_position_in_buffer += Player::LengthToSend();
      }
    }
  }
}

}