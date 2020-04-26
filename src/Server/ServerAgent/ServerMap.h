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
#include "UDPToPhysicsQueue.h"
#include "PhysicsToMapQueue.h"


namespace Server {

class ServerMap {
 public:
  ServerMap() = default;

  void LoadImageFromFile(const std::string &image_file);

  const sf::Image &GetImage();

  unsigned AddPlayer(); // get new player id

  void DeletePlayer(unsigned int player_id, bool is_already_lock);

  TCPSocketHelper::ConstBuffer GetCurrentInfo();

  [[nodiscard]] Location GetPlayerLocation(unsigned player_id);

  virtual ~ServerMap() = default;

  TCPSocketHelper::ConstBuffer SynchronizeAndPrepareSendString();

  size_t NumberOfPlayers();

  bool WasSynchronized();

 private:
  void HandleDeletedPlayer(unsigned player_id, char* current_position_in_buffer);

 public:
  UDPToPhysicsQueue udp_to_physics_queue_;
  PhysicsToMapQueue physics_to_map_queue_;

 private:
  std::vector<std::unique_ptr<Player>> players_;
  std::shared_mutex mutex_players_;
  sf::Image background_;
  bool was_synchronized_ = false;
  unsigned number_of_players_ = 0;

};

}  // namespace Server

#endif  // GAME_SRC_MAP_H_
