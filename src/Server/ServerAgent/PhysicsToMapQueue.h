//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_PHYSICSTOMAPQUEUE_H_
#define GAME_SRC_SERVER_SERVERAGENT_PHYSICSTOMAPQUEUE_H_

#include <queue>
#include <mutex>
#include "PlayerState.h"


namespace Server {

class PhysicsToMapQueue {
 public:
  PhysicsToMapQueue();
  int Initialize();
  void PushBack(const PlayerState &player_state);
  std::queue<PlayerState> GetQueue();
  void Close();

 private:
  std::queue<PlayerState> players_state_;
  std::mutex mutex_;
  int event_fd_ = -1;

  static constexpr size_t kMaxElementInQueue = 30;
};

}
#endif //GAME_SRC_SERVER_SERVERAGENT_PHYSICSTOMAPQUEUE_H_
