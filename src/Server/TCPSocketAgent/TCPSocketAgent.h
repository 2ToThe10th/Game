//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
#define GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_

#include <thread>
#include "../ServerAgent/ServerMap.h"


namespace Server::TCPSocketAgent {

class TCPSocketAgent {
 public:
  explicit TCPSocketAgent(ServerMap &main_map);

  void Initialize(size_t port);

  void Close();

 private:
  Server::ServerMap &main_map_;
  std::thread tcp_accept_thread_;
  bool is_work_ = true;
  int accept_socket_ = -1;

  static constexpr size_t kMaxReadForOneTime = 1024;

 private:
  void AcceptLoop();
  void SendImage(int socket);
};

}

#endif //GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
