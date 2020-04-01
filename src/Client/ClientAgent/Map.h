//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#ifndef GAME_SRC_MAP_H_
#define GAME_SRC_MAP_H_

#include <SFML/Graphics/Image.hpp>
#include <shared_mutex>
#include <vector>

#include "../../Player.h"


namespace Client {

class Map {
 public:
  Map() = default;

  void AddPlayer(const Player &new_player, size_t new_id);

  void DeletePlayer(size_t player_id);

  void SetPlayerLocation(size_t player_id, Location new_location);

  [[nodiscard]] Location GetPlayerLocation(size_t player_id);

  virtual ~Map() = default;

 private:
  std::vector<Player> players_;
  std::shared_mutex mutex_;
};

}  // namespace Client

#endif  // GAME_SRC_MAP_H_
