//
// Created by 2ToThe10th on 02.04.2020.
//

#include <iostream>
#include <cassert>
#include "UDPSocketAgent.h"


namespace Client::UDPSocketAgent {

UDPSocketAgent::UDPSocketAgent(ClientMap &main_map) : main_map_(main_map) {

}

void UDPSocketAgent::Initialize(const std::string &host, const size_t port) {
//TODO
}

void UDPSocketAgent::WriteToServer(const std::string &message) {
  assert(message.size() == 1);
  std::cout << int(message[0]) << std::endl;
//TODO
}

void UDPSocketAgent::Close() {
//TODO
}

}