//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_SERVERAGENT_H_
#define GAME_SRC_SERVER_SERVERAGENT_SERVERAGENT_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include "../TCPSocketAgent/TCPSocketAgent.h"
#include "../UDPSocketAgent/UDPSocketAgent.h"
#include "../PhysicalAgent/PhysicalAgent.h"
#include "ServerMap.h"


namespace Server {

class ServerAgent {
 public:
  explicit ServerAgent();

  void InitServer(const std::string &host, size_t tcp_port, size_t udp_port);

  void RunServer();

  void Close();

 private:
  ServerMap map_;
  PhysicalAgent::PhysicalAgent physical_agent_;
  TCPSocketAgent::TCPSocketAgent tcp_socket_agent_;
  UDPSocketAgent::UDPSocketAgent udp_socket_agent_;
};

}

#endif //GAME_SRC_SERVER_SERVERAGENT_SERVERAGENT_H_
