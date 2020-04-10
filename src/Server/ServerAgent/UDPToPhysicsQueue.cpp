//
// Created by 2ToThe10th on 10.04.2020.
//

#include "UDPToPhysicsQueue.h"


namespace Server {

void UDPToPhysicsQueue::PushBack(UserUpdate user_update) {
  std::unique_lock lock_(mutex_);

  condition_variable_.notify_one();

  user_updates_.emplace(user_update);

}

UserUpdate UDPToPhysicsQueue::PopFront(const bool &is_work_) {
  std::unique_lock lock_(mutex_);

  while(user_updates_.empty()) {
    condition_variable_.wait_for(lock_, kTimeout);
    if (!is_work_) {
      return UserUpdate(0, UserAction::Nothing());
    }
  }

  auto front_user_update = user_updates_.front();
  user_updates_.pop();

  return front_user_update;
}

}