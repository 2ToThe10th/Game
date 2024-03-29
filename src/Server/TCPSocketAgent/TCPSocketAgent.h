//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
#define GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_

#include <thread>

#include "../../Duration.h"
#include "../../EpollOneReturn.h"
#include "../ServerAgent/ServerMap.h"

namespace Server::TCPSocketAgent {

class TCPSocketAgent {
public:
  explicit TCPSocketAgent(ServerMap &main_map);

  void Initialize(size_t port);

  void Close();

private:
  void AcceptLoop();
  void SendImage(int socket);
  void SetAndSendPlayerId(int client_socket);

  void CheckHashAndWriteLoop();

  static std::vector<uint64_t>
  HashVectorFromBuffer(TCPSocketHelper::ConstBuffer &buffer);

private:
  Server::ServerMap &main_map_;
  std::thread tcp_accept_thread_;
  std::thread tcp_write_thread_;
  EpollOneReturn client_epoll_;
  bool is_work_ = true;
  int accept_socket_ = -1;

  static constexpr size_t kEpollTimeoutMillisecond = 1000;
  static constexpr Time::Duration kWriteAllPeriod = Time::Duration(1000);
};

} // namespace Server::TCPSocketAgent

#endif // GAME_SRC_SERVER_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
