//
// Created by 2ToThe10th on 23.03.2020.
//

#include <thread>
#include <iostream>

#include "GameClient.h"
#include "GameGraphic.h"

GameClient::GameClient(const std::string &host,
                       size_t udp_port,
                       size_t tcp_port,
                       sf::Window &window) {
  Map map;

  sf::Image image;
  if (!image.loadFromFile("~/Game/res/Map.png")) {
    std::cout << "oy" << std::endl;
    return;
  }

  map.LoadImage(std::move(image));

  GameGraphic game_graphic(map, window);

  game_graphic.GameLoop();
}
