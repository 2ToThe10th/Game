//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
#define GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_

#include <thread>

#include "../ServerAgent/ServerMap.h"


namespace Server::TCPSocketAgent {

using Duration = std::chrono::milliseconds;

class TCPSocketAgent {
 public:
  explicit TCPSocketAgent(ServerMap &main_map);

  void Initialize(size_t port);

  void Close();

 private:
  Server::ServerMap &main_map_;
  std::thread tcp_accept_thread_;
  std::thread tcp_write_thread_;
  std::vector<int> client_sockets_;
  bool is_work_ = true;
  int accept_socket_ = -1;

  static constexpr size_t kMaxReadForOneTime = 1024;
  static constexpr size_t kEpollTimeoutMillisecond = 1000;
  static constexpr Duration kWriteAllPeriod = Duration(1000);

 private:
  void AcceptLoop();
  void SendImage(int socket);
  void SetAndSendPlayerId(int client_socket);
  void SendCurrentSituation(int client_socket);

  void WriteLoop();
};

}

#endif //GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
