//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_UDPTOPHYSICSQUEUE_H_
#define GAME_SRC_SERVER_SERVERAGENT_UDPTOPHYSICSQUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include "../../UserAction.h"
#include "../../Duration.h"
#include "UserUpdate.h"


namespace Server {

class UDPToPhysicsQueue {
 public:

  void PushBack(UserUpdate user_update);

  UserUpdate PopFront(const bool &is_work_);

 private:

  std::queue<UserUpdate> user_updates_;
  std::mutex mutex_;
  std::condition_variable condition_variable_;

  static constexpr Time::Duration kTimeout = Time::Duration(100);
};

}

#endif //GAME_SRC_SERVER_SERVERAGENT_UDPTOPHYSICSQUEUE_H_
