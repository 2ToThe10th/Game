//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_GAMECLIENT_H_
#define GAME_SRC_GAMECLIENT_H_

#include <cstdlib>
#include <string>
#include <SFML/Graphics.hpp>

#include "Map.h"

class GameClient {
 public:
  GameClient(const std::string &host, size_t udp_port, size_t tcp_port, sf::Window &window);
 private:
  Map map_;
  // Two Sockets UDP and TCP
 private:
};

#endif //GAME_SRC_GAMECLIENT_H_
