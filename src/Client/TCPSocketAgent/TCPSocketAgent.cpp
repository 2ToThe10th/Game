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


namespace Client::TCPSocketAgent {
TCPSocketAgent::TCPSocketAgent(Map &main_map) : main_map_(main_map) {

}

class InetAtonExeption : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "inet aton";
  }
};

void TCPSocketAgent::Initialize(const std::string &host, const size_t port, sf::Image &image) {

  Connect(host, port);

  // TODO: Read image, get current situation

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
    throw InetAtonExeption();
  }

  if (connect(socket_, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
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