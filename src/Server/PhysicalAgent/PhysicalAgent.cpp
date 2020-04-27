//
// Created by 2ToThe10th on 03.04.2020.
//

#include <iostream>
#include "PhysicalAgent.h"


namespace Server::PhysicalAgent {

PhysicalAgent::PhysicalAgent(ServerMap &main_map) : main_map_(main_map) {
}

void PhysicalAgent::Initialize(size_t number_of_thread) {
  map_size_x_ = main_map_.GetImage().getSize().x;
  map_size_y_ = main_map_.GetImage().getSize().y;
  for (size_t i = 0; i < number_of_thread; ++i) {
    threads.emplace_back([this] {
      this->PhysicalLoop();
    });
  }
}

void PhysicalAgent::Close() {
  is_work_ = false;
  for (auto &thread: threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void PhysicalAgent::PhysicalLoop() {
  while (is_work_) {
    UserUpdate user_update = main_map_.udp_to_physics_queue_.PopFront(is_work_);
    if (!is_work_) {
      break;
    }
    HandleUpdate(user_update);
  }
}

void PhysicalAgent::HandleUpdate(UserUpdate user_update) { //TODO: refactor

  if (user_update.GetUserAction().IsCommandMessage()) {
    main_map_.physics_to_map_queue_.PushBack(PlayerState(user_update.GetPlayerId(),
                                                         user_update.GetUserAction().GetCommand()));
    return;
  }

  struct Delta {
    float x;
    float y;
  };

  constexpr Delta kDelta[] =
      {{0, 0}, {0, -kGoRightForOneTick}, {kGoDiagonalForOneTick, -kGoDiagonalForOneTick},
       {kGoRightForOneTick, 0}, {kGoDiagonalForOneTick, kGoDiagonalForOneTick},
       {0, kGoRightForOneTick}, {-kGoDiagonalForOneTick, kGoDiagonalForOneTick},
       {-kGoRightForOneTick, 0}, {-kGoDiagonalForOneTick, -kGoDiagonalForOneTick}, {0, 0}};

  std::lock_guard lock(mutex_is_player_move_);

  if (main_map_.WasSynchronized()) {
    OnSynchronize();
  }

  if (is_player_move_.size() != main_map_.NumberOfPlayers()) {
    is_player_move_.resize(main_map_.NumberOfPlayers(), false);
  }

  if (is_player_move_[user_update.GetPlayerId()]) {
    return;
  }
  is_player_move_[user_update.GetPlayerId()] = true;

  mutex_is_player_move_.unlock();

  auto current_location = main_map_.GetPlayerLocation(user_update.GetPlayerId());

  current_location.SetX(
      current_location.GetX() + kDelta[static_cast<int>(user_update.GetUserAction().GetAngle())].x);
  current_location.SetY(
      current_location.GetY() + kDelta[static_cast<int>(user_update.GetUserAction().GetAngle())].y);

  if (IsOnMap(current_location)) {
    main_map_.physics_to_map_queue_.PushBack(PlayerState(user_update.GetPlayerId(),
                                                         current_location));
  }
}

bool PhysicalAgent::IsOnMap(const Location &location) const {
  return !(location.GetX() < 0 || location.GetY() < 0 || location.GetX() > map_size_x_
      || location.GetY() > map_size_y_);
}

void PhysicalAgent::OnSynchronize() {
  for (auto i : is_player_move_) {
    i = false;
  }
}

}