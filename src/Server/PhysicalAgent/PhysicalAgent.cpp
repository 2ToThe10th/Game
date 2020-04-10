//
// Created by 2ToThe10th on 03.04.2020.
//

#include "PhysicalAgent.h"


namespace Server::PhysicalAgent {

PhysicalAgent::PhysicalAgent(ServerMap &main_map) : main_map_(main_map) {
}

void PhysicalAgent::Initialize(size_t number_of_thread) {
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

void PhysicalAgent::HandleUpdate(UserUpdate user_update) {
  auto current_location = main_map_.GetPlayerLocation(user_update.GetPlayerId());
  switch (user_update.GetUserAction().GetAngle()) {
    case Angle::Up:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX(),
                               current_location.GetY() - kGoRightForOneTick)));
      break;
    case Angle::Down:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX(),
                               current_location.GetY() + kGoRightForOneTick)));
      break;
    case Angle::Right:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() + kGoRightForOneTick,
                               current_location.GetY())));
      break;
    case Angle::Left:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() - kGoRightForOneTick,
                               current_location.GetY())));
      break;
    case Angle::UpLeft:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() - kGoDiagonalForOneTick,
                               current_location.GetY() - kGoDiagonalForOneTick)));
      break;
    case Angle::UpRight:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() + kGoDiagonalForOneTick,
                               current_location.GetY() - kGoDiagonalForOneTick)));
      break;
    case Angle::DownLeft:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() - kGoDiagonalForOneTick,
                               current_location.GetY() + kGoDiagonalForOneTick)));
      break;
    case Angle::DownRight:
      main_map_.physics_to_map_queue_.PushBack(
          PlayerState(user_update.GetPlayerId(),
                      Location(current_location.GetX() + kGoDiagonalForOneTick,
                               current_location.GetY() + kGoDiagonalForOneTick)));
      break;
    case Angle::None:
      break;
  }
}

}