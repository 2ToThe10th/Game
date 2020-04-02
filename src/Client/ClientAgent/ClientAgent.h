//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_GAMECLIENT_H_
#define GAME_SRC_GAMECLIENT_H_

#include <cstdlib>
#include <string>
#include <SFML/Graphics.hpp>

#include "Map.h"
#include "../GraphicAgent/GraphicAgent.h"
#include "../InputAgent/InputAgent.h"
#include "../TCPSocketAgent/TCPSocketAgent.h"
#include "../UDPSocketAgent/UDPSocketAgent.h"


namespace Client {

class ClientAgent {
 public:
  explicit ClientAgent(sf::RenderWindow &window);

  void InitGame(const std::string &host, size_t tcp_port, size_t udp_port);

  void RunGame();

  void Close();

 private:
  sf::RenderWindow& window_;
  Map map_;
  GraphicAgent::GraphicAgent graphic_agent_;
  InputAgent::InputAgent input_agent_;
  TCPSocketAgent::TCPSocketAgent tcp_socket_agent_;
  UDPSocketAgent::UDPSocketAgent udp_socket_agent_;
};

} // namespace client

#endif //GAME_SRC_GAMECLIENT_H_
