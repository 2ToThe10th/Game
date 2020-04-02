//
// Created by 2ToThe10th on 23.03.2020.
//
#include <cassert>
#include <cstdlib>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Client/ClientAgent/ClientAgent.h"


#ifdef DEBUG
#include <gtest/gtest.h>
#endif

int main(int argc, char *argv[]) {

#ifdef DEBUG
  ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

#ifdef SERVER
  return 0;
#endif
#ifdef CLIENT
  assert(argc == 4);
  sf::RenderWindow
      window
      (sf::VideoMode(600, 400), "Game", sf::Style::Close | sf::Style::Resize);
  Client::ClientAgent game_client(window);
  game_client.InitGame(argv[1],
                       strtoul(argv[2], nullptr, 10),
                       strtoul(argv[3], nullptr, 10));
  game_client.RunGame();
  game_client.Close();
  return 0;
#endif
}