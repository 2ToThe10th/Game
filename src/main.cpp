//
// Created by 2ToThe10th on 23.03.2020.
//
#include <cassert>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <iostream>


#ifdef CLIENT
#include "Client/ClientAgent/ClientAgent.h"


#endif

#ifdef SERVER
#include "Server/ServerAgent/ServerAgent.h"


#endif

#ifdef DEBUG
#include <gtest/gtest.h>
#endif

int main(int argc, char *argv[]) {

#ifdef DEBUG
  ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

#ifdef SERVER
  assert(argc == 4);

  Server::ServerAgent server_agent;

  server_agent.InitServer(strtoul(argv[1], nullptr, 10),
                          strtoul(argv[2], nullptr, 10), argv[3]);

  server_agent.RunServer();

  server_agent.Close();

  return 0;
#endif

#ifdef CLIENT
  assert(argc == 4);
  sf::RenderWindow
      window
      (sf::VideoMode(700, 600), "Game", sf::Style::Close | sf::Style::Resize);
  Client::ClientAgent game_client(window);
  game_client.InitGame(argv[1],
                       strtoul(argv[2], nullptr, 10),
                       strtoul(argv[3], nullptr, 10));
  game_client.RunGame();
  game_client.Close();
  return 0;
#endif
}