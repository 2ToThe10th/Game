//
// Created by 2ToThe10th on 23.03.2020.
//
#include <cassert>
#include <cstdlib>

#include "GameClient.h"
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
  sf::Window window(sf::VideoMode(1600, 1200), "Game", sf::Style::Close);
  GameClient game_client(argv[1],
                         strtoul(argv[2], nullptr, 10),
                         strtoul(argv[3], nullptr, 10),
                         window);
  return 0;
#endif
}