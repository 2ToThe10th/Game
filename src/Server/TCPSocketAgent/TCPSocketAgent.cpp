//
// Created by 2ToThe10th on 03.04.2020.
//

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TCPSocketAgent.h"
#include "../../TCPSocketHelper.h"


namespace Server::TCPSocketAgent {

TCPSocketAgent::TCPSocketAgent(ServerMap &main_map) : main_map_(main_map) {

}

void TCPSocketAgent::Initialize(size_t port) {
  accept_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (inet_aton("127.0.0.1", &addr.sin_addr) != 1) {
    throw TCPSocketHelper::InetAtonExeption();
  }

  if (bind(accept_socket_, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  if (listen(accept_socket_, 128) != 0) {
    throw std::system_error(errno, std::generic_category());
  }

  tcp_accept_thread_ = std::thread([this] {
    this->AcceptLoop();
  });
}

void TCPSocketAgent::SendImage(int socket) {
  int height = main_map_.GetImage().getSize().y;
  int width = main_map_.GetImage().getSize().x;
  TCPSocketHelper::WriteAll(socket, (char *) &height, sizeof(int));
  TCPSocketHelper::WriteAll(socket, (char *) &width, sizeof(int));
  TCPSocketHelper::WriteAll(socket,
                            (char *) main_map_.GetImage().getPixelsPtr(),
                            4 * height * width);
}

void TCPSocketAgent::AcceptLoop() {
  for (;;) {
    int client_socket = accept(accept_socket_, nullptr, nullptr);

    SendImage(client_socket);

    break;
  }
}

void TCPSocketAgent::Close() {
  close(accept_socket_);
}

}