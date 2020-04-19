//
// Created by 2ToThe10th on 03.04.2020.
//

#include <iostream>
#include "ServerAgent.h"


namespace Server {

ServerAgent::ServerAgent()
    : map_(), physical_agent_(map_), tcp_socket_agent_(map_), udp_socket_agent_(map_) {

}

void ServerAgent::InitServer(size_t tcp_port, size_t udp_port, const std::string &image_file) {
  map_.LoadImageFromFile(image_file);

  tcp_socket_agent_.Initialize(tcp_port);
  int event_fd = map_.physics_to_map_queue_.Initialize();
  udp_socket_agent_.Initialize(udp_port, event_fd);
  physical_agent_.Initialize(1);
}

void ServerAgent::RunServer() {
  int t;
  std::cin >> t;
}

void ServerAgent::Close() {
  physical_agent_.Close();
  tcp_socket_agent_.Close();
  udp_socket_agent_.Close();
  map_.physics_to_map_queue_.Close();
}

}