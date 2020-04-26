//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_SERVER_PHYSICALAGENT_PHYSICALAGENT_H_
#define GAME_SRC_SERVER_PHYSICALAGENT_PHYSICALAGENT_H_

#include <thread>
#include "../ServerAgent/ServerMap.h"


namespace Server::PhysicalAgent {

class PhysicalAgent {
 public:
  explicit PhysicalAgent(ServerMap &main_map);

  void Initialize(size_t number_of_thread);

  void Close();

 private:
  ServerMap& main_map_;
  std::vector<std::thread> threads;
  std::vector<bool> is_player_move_;
  std::mutex mutex_is_player_move_;
  bool is_work_ = true;
  static constexpr float kGoRightForOneTick = 0.2;
  static constexpr float kGoDiagonalForOneTick = kGoRightForOneTick / 1.41421356237;
  static constexpr float start_position_x_ = 100;
  static constexpr float start_position_y_ = 100;
  float map_size_x_ = 0;
  float map_size_y_ = 0;
 private:

  void PhysicalLoop();
  void HandleUpdate(UserUpdate user_update);
  void OnSynchronize();

  [[nodiscard]] bool IsOnMap(const Location& location) const;
};

}

#endif //GAME_SRC_SERVER_PHYSICALAGENT_PHYSICALAGENT_H_
