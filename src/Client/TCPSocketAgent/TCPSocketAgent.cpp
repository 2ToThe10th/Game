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

void TCPSocketAgent::Initialize(const std::string &host, const size_t port, sf::Image &image) {

  Connect(host, port);

  ReceiveImage(image);

  tcp_read_thread_ = std::thread([this]() {
    this->RunTCPRead();
  });
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
    throw TCPSocketHelper::InetAtonExeption();
  }

  if (connect(socket_, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void TCPSocketAgent::ReceiveImage(sf::Image &image) {
  int height = 0;
  int width = 0;
  TCPSocketHelper::ReadAll(socket_, (char *) &height, sizeof(int));
  TCPSocketHelper::ReadAll(socket_, (char *) &width, sizeof(int));
  auto array_of_image = new sf::Uint8[height * width * 4];
  TCPSocketHelper::ReadAll(socket_, (char *) array_of_image, height * width * 4);
  image.create(width, height, array_of_image);
}

void TCPSocketAgent::Close() {
  is_work_ = false;
  tcp_read_thread_.join();
  shutdown(socket_, SHUT_RDWR);
  close(socket_);
}

void TCPSocketAgent::RunTCPRead() {
  char buffer[kMaxReadForOneTime + 1];//TODO: delete + 1
  while (is_work_) {
    size_t was_read = read(socket_, buffer, kMaxReadForOneTime);

    buffer[was_read] = '\0';

    std::cout << "------------------------------\n" << was_read << "\n" << buffer
              << "\n------------------------------" << std::endl;
  }
}

}