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
#include "Server/TCPSocketAgent/TCPSocketAgent.h"
#include "Server/ServerAgent/ServerMap.h"


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

  Server::ServerMap map_;
  Server::TCPSocketAgent::TCPSocketAgent tcp_socket_agent(map_);

  map_.LoadImageFromFile(argv[3]);

  tcp_socket_agent.Initialize(
      strtoul(argv[1], nullptr, 10));

  int t;
  std::cin >> t;

  tcp_socket_agent.Close();

  return 0;
#endif

#ifdef CLIENT
  assert(argc == 4);
  sf::RenderWindow
      window
      (sf::VideoMode(1000, 800), "Game", sf::Style::Close | sf::Style::Resize);
  Client::ClientAgent game_client(window);
  game_client.InitGame(argv[1],
                       strtoul(argv[2], nullptr, 10),
                       strtoul(argv[3], nullptr, 10));
  game_client.RunGame();
  game_client.Close();
  return 0;
#endif
}