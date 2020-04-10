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


namespace Client {

class ClientMap {
 public:
  ClientMap() = default;

  void UpdatePlayer(unsigned int player_id, char *buffer);

  void UpdateByConstBuffer(TCPSocketHelper::ConstBuffer &const_buffer);

  virtual ~ClientMap() = default;

 private:
 public: //TODO: delete
  std::vector<Player *> players_;
  std::mutex mutex_;
  size_t number_of_changes_ = 0;

 private:
  void DeletePlayersFromTo(unsigned int from, unsigned int to);
};

}  // namespace Client

#endif  // GAME_SRC_MAP_H_
