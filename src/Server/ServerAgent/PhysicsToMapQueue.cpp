//
// Created by 2ToThe10th on 10.04.2020.
//

#include "PhysicsToMapQueue.h"
#include <unistd.h>
#include <sys/eventfd.h>
#include <iostream>
#include <iomanip>


namespace Server {

PhysicsToMapQueue::PhysicsToMapQueue() = default;

void PhysicsToMapQueue::PushBack(const PlayerState &player_state) {
  std::lock_guard lock(mutex_);
  players_state_.push(player_state);
  if(players_state_.size() > kMaxElementInQueue) {
    uint64_t message = 1;
    write(event_fd_, &message, sizeof(message));
  }
}

std::queue<PlayerState> PhysicsToMapQueue::GetQueue() {
  std::lock_guard lock(mutex_);
  std::queue<PlayerState> new_queue;
  players_state_.swap(new_queue);
  return std::move(new_queue);
}

int PhysicsToMapQueue::Initialize() {
  event_fd_ = eventfd(0, 0);
  if (event_fd_ < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return event_fd_;
}

void PhysicsToMapQueue::Close() {
  close(event_fd_);
}

}