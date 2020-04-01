//
// Created by 2ToThe10th on 23.03.2020.
//

#include "ClientAgent.h"

#include <iostream>
#include <thread>

namespace Client {

void ClientAgent::InitGame(const std::string &host,
                           size_t udp_port,
                           size_t tcp_port,
                           sf::RenderWindow &window) {
  Map map;

  sf::Image image;
  if (!image.loadFromFile("/home/sasha/Game/res/Map.png")) {
    std::cout << "oy" << std::endl;
    return;
  }
//  GraphicAgent::GraphicAgent game_graphic(map, window);

//  game_graphic.GameLoop();
}

} // namespace Client