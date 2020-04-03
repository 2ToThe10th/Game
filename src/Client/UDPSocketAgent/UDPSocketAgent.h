//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENT_UDPSOCKETAGENT_UDPSOCKETAGENT_H_
#define GAME_SRC_CLIENT_UDPSOCKETAGENT_UDPSOCKETAGENT_H_

#include <string>
#include "../ClientAgent/ClientMap.h"


namespace Client::UDPSocketAgent {

class UDPSocketAgent {
 public:
  explicit UDPSocketAgent(ClientMap &main_map);

  void Initialize(const std::string &host, size_t port);

  void WriteToServer(const std::string &message);

  void Close();

 private:
  ClientMap &main_map_;
};

}

#endif //GAME_SRC_CLIENT_UDPSOCKETAGENT_UDPSOCKETAGENT_H_