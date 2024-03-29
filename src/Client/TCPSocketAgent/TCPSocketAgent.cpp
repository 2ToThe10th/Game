//
// Created by 2ToThe10th on 02.04.2020.
//

#include "TCPSocketAgent.h"
#include "../../EpollOneReturn.h"
#include "../../TCPSocketHelper.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

namespace Client::TCPSocketAgent {
TCPSocketAgent::TCPSocketAgent(ClientMap &main_map) : main_map_(main_map) {}

unsigned TCPSocketAgent::Initialize(const std::string &host, const size_t port,
                                    sf::Image &image) {

  Connect(host, port);

  ReceiveImage(image);

  unsigned player_id;
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *>(&player_id),
                           sizeof(player_id));

  tcp_read_thread_ = std::thread([this]() { this->RunTCPRead(); });

  return player_id;
}

void TCPSocketAgent::Connect(const std::string &host, size_t port) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in server_address {};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  if (inet_aton(host.c_str(), &server_address.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonException();
  }

  if (connect(socket_, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void TCPSocketAgent::ReceiveImage(sf::Image &image) {
  unsigned height = 0;
  unsigned width = 0;
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *>(&height),
                           sizeof(height));
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *>(&width),
                           sizeof(width));
  auto array_of_image = new sf::Uint8[height * width * 4];
  TCPSocketHelper::ReadAll(socket_, reinterpret_cast<char *>(array_of_image),
                           height * width * 4);
  image.create(width, height, array_of_image);
  delete[] array_of_image;
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

  auto epoll = EpollOneReturn();

  epoll.Add(socket_);

  while (is_work_) { // передает hash карты и тайлов и отправляет ключевой кадр
                     // тайла только там где hash не совпадает
    if (epoll.Wait(kTimeoutMillisecond)) {
      auto buffer = GetCurrentSituation();
      main_map_.UpdateByConstBuffer(buffer);
    } else {
      main_map_.GetHash().WriteTo(socket_);
    }
  }
}

TCPSocketHelper::ConstBuffer TCPSocketAgent::GetCurrentSituation() const {
  return TCPSocketHelper::ConstBuffer::ReadFrom(socket_);
}

} // namespace Client::TCPSocketAgent