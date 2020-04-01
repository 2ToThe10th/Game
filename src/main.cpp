//
// Created by 2ToThe10th on 23.03.2020.
//
#include <cassert>
#include <cstdlib>

#include "ClientAgent/ClientAgent.h"


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
      (sf::VideoMode(6400, 1360), "Game", sf::Style::Close | sf::Style::Resize);
  Client::ClientAgent game_client;
  game_client.RunGame(argv[1],
                      strtoul(argv[2], nullptr, 10),
                      strtoul(argv[3], nullptr, 10),
                      window);
  return 0;
#endif
}