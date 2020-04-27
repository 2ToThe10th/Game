//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENT_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
#define GAME_SRC_CLIENT_TCPSOCKETAGENT_TCPSOCKETAGENT_H_

#include <string>
#include <thread>
#include "../ClientAgent/ClientMap.h"
#include "../../TCPSocketHelper.h"


namespace Client::TCPSocketAgent {

class TCPSocketAgent {
 public:
  explicit TCPSocketAgent(ClientMap &main_map);

  [[nodiscard]] unsigned Initialize(const std::string &host, size_t port, sf::Image &image);

  void Close();

 private:
  ClientMap &main_map_;
  std::thread tcp_read_thread_;
  bool is_work_ = true;
  int socket_ = -1;

  static constexpr size_t kMaxReadForOneTime = 1024;
  static constexpr size_t kTimeoutMillisecond = 1000;

 private:
  void Connect(const std::string &host, size_t port);
  void RunTCPRead();
  void ReceiveImage(sf::Image &image);
  [[nodiscard]] TCPSocketHelper::ConstBuffer GetCurrentSituation() const;
};

}

#endif //GAME_SRC_CLIENT_TCPSOCKETAGENT_TCPSOCKETAGENT_H_
