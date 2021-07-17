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
#include <iostream>

#include "../../Player.h"
#include "../../TCPSocketHelper.h"


namespace Client {

class ClientMap {
 public:
  ClientMap() = default;

  void SetMapSize(sf::Vector2u size);

  void UpdatePlayer(unsigned int player_id, char *buffer);

  void UpdateByConstBuffer(TCPSocketHelper::ConstBuffer &const_buffer);

  template<typename Map>
  bool Synchronize(Map &map);

  TCPSocketHelper::ConstBuffer GetHash();

  virtual ~ClientMap() = default;

private:
  void DeletePlayer(unsigned int player_id);
  void AddPlayerIfNotExist(unsigned player_id);
  void DeletePlayerIfExist(unsigned player_id);
  unsigned int UpdateExistPlayers(TCPSocketHelper::ConstBuffer &const_buffer);

 private:
  std::vector<std::unique_ptr<Player>> players_;
  std::mutex mutex_;
  size_t number_of_changes_ = 0;
  unsigned map_x_size_ = 0;
  unsigned map_y_size_ = 0;

};

template<typename Map>
bool ClientMap::Synchronize(Map &map) {
  std::lock_guard lock(mutex_);

  if (map.LastSynchronize() == number_of_changes_) {
    return false;
  }

  map.PlayerVector().resize(players_.size());

  for (int index = 0; index < players_.size(); ++index) {
    if (players_[index] == nullptr) {
      if (map.PlayerVector()[index] != nullptr) {
        map.PlayerVector()[index] = nullptr;
      }
    } else {
      if (map.PlayerVector()[index] == nullptr) {
        map.PlayerVector()[index] = std::make_unique<Player>();
      }
      map.PlayerVector()[index]->UpdateFromPlayer(*players_[index]);
    }
  }

  map.LastSynchronize() = number_of_changes_;
  return true;
}


}  // namespace Client

#endif  // GAME_SRC_MAP_H_
