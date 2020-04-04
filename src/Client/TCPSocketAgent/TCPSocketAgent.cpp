//
// Created by 2ToThe10th on 02.04.2020.
//

#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "TCPSocketAgent.h"
#include "../../TCPSocketHelper.h"


namespace Client::TCPSocketAgent {
TCPSocketAgent::TCPSocketAgent(ClientMap &main_map) : main_map_(main_map) {

}

size_t TCPSocketAgent::Initialize(const std::string &host, const size_t port, sf::Image &image) {

  Connect(host, port);

  ReceiveImage(image);

  std::cout << "Start Reading" << std::endl;

  size_t player_id;
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *> (&player_id), sizeof(player_id));

  std::cout << "End Reading" << std::endl;

  tcp_read_thread_ = std::thread([this]() {
    this->RunTCPRead();
  });

  return player_id;
}

void TCPSocketAgent::Connect(const std::string &host, size_t port) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  if (inet_aton(host.c_str(), &server_address.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonException();
  }

  if (connect(socket_, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void TCPSocketAgent::ReceiveImage(sf::Image &image) {
  int height = 0;
  int width = 0;
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *> (&height), sizeof(int));
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *> (&width), sizeof(int));
  auto array_of_image = new sf::Uint8[height * width * 4];
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *> (array_of_image), height * width * 4);
  image.create(width, height, array_of_image);
}

void TCPSocketAgent::Close() {
  is_work_ = false;
  if (tcp_read_thread_.joinable()) {
    tcp_read_thread_.join();
  }
  shutdown(socket_, SHUT_RDWR);
  close(socket_);
}

void TCPSocketAgent::RunTCPRead() {

  auto epoll = TCPSocketHelper::EpollOneReturn();

  epoll.Add(socket_, nullptr);

  while (is_work_) {
    if (epoll.Wait(kTimeoutMillisecond)) {
      auto buffer = GetCurrentSituation();
      main_map_.UpdateByConstBuffer(buffer);
    }
  }
}

TCPSocketHelper::ConstBuffer TCPSocketAgent::GetCurrentSituation() {
  return TCPSocketHelper::ConstBuffer::ReadFrom(socket_);
}

}