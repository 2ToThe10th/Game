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
#include "../../TCPSocketHelper.h"


namespace Server {

class ServerMap {
 public:
  ServerMap() = default;

  void LoadImageFromFile(const std::string &image_file);

  const sf::Image& GetImage();

  size_t AddPlayer(); // get new player id

  void DeletePlayer(size_t player_id);

  TCPSocketHelper::ConstBuffer GetCurrentInfo();

  void SetPlayerLocation(size_t player_id, Location new_location);
//
//  [[nodiscard]] Location GetPlayerLocation(size_t player_id);

  virtual ~ServerMap() = default;

 private:
  std::vector<Player*> players_;
  std::shared_mutex mutex_;
  sf::Image background_;
  size_t number_of_players_ = 0;
  // + 2 queue for Agents

};

}  // namespace Server

#endif  // GAME_SRC_MAP_H_
