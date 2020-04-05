//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_
#define GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_

#include "../ServerAgent/ServerMap.h"
#include <netinet/in.h>
#include <thread>


namespace Server::UDPSocketAgent {

class UDPSocketAgent {
 public:
  explicit UDPSocketAgent(ServerMap &main_map);

  void Initialize(size_t port);

  void Close();

 private:
  ServerMap &main_map_;
  int socket_ = -1;
  std::thread udp_read_thread;
  bool is_work_ = true;

  struct ClientAddr {
    ClientAddr() = default;

    struct sockaddr *addr_ = nullptr;
    size_t size_ = 0;

    bool IsEmpty();
  };

  std::vector<ClientAddr> client_addresses_;

  static constexpr size_t kTimeoutMillisecond = 1000;
  static constexpr size_t kMaxBufferSize = 1024;

 private:
  void ReadLoop();
};

}

#endif //GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_
