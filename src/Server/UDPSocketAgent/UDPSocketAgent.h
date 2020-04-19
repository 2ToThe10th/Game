//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_
#define GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_

#include "../ServerAgent/ServerMap.h"
#include <netinet/in.h>
#include <thread>
#include <cstring>


namespace Server::UDPSocketAgent {

class UDPSocketAgent {
 public:
  explicit UDPSocketAgent(ServerMap &main_map);

  void Initialize(size_t port, int event_fd);

  void Close();

 private:
  ServerMap &main_map_;
  int socket_ = -1;
  std::thread udp_read_thread;
  std::thread udp_write_thread;
  bool is_work_ = true;

  struct ClientAddr {
    ClientAddr();

    struct sockaddr_in addr_{};
    socklen_t size_ = 0;

    bool IsEmpty();

    bool IsEqual(sockaddr_in *addr, socklen_t socklen);

    void SetFrom(sockaddr_in *addr, socklen_t socklen);
  };

  std::vector<ClientAddr> client_addresses_;

  static constexpr size_t kTimeoutEpollRead = 1000; // Millisecond
  static constexpr size_t kTimeoutWrite = 5; // Millisecond
  static constexpr size_t kMaxBufferSize = 1024;

 private:
  void ReadLoop();
  void WriteLoop(int event_fd);
};

}

#endif //GAME_SRC_SERVER_UDPSOCKETAGENT_UDPSOCKETAGENT_H_
