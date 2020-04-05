//
// Created by 2ToThe10th on 02.04.2020.
//

#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstring>
#include "UDPSocketAgent.h"
#include "../../UDPSocketHelper.h"


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
}

void UDPSocketAgent::WriteToServer(const std::string &message) {
  size_t real_size = message.size() + sizeof(player_id_);
  char real_message[real_size];
  memcpy(real_message, &player_id_, sizeof(player_id_));
  memcpy(real_message + sizeof(player_id_), message.c_str(), message.size());
  UDPSocketHelper::WriteAll(socket_, server_addr_, real_message, real_size);
}

void UDPSocketAgent::Close() {
//TODO
}

}