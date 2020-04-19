//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ClientMap.h"

#include <iostream>
#include <mutex>
#include <cstring>


namespace Client {

void ClientMap::UpdateByConstBuffer(TCPSocketHelper::ConstBuffer &const_buffer) {
  std::lock_guard lock(mutex_);
  unsigned
      player_in_buffer_number = const_buffer.GetSize() / (sizeof(unsigned) + 2 * sizeof(float));

  char *buffer = const_buffer.GetBuffer();

  unsigned after_last_index = 0;

  for (unsigned i = 0; i < player_in_buffer_number; ++i) {
    unsigned player_index = 0;

    memcpy(&player_index, buffer, sizeof(player_index));
    buffer += sizeof(player_index);

    if (player_index >= players_.size()) {
      players_.resize(player_index + 1);
    }

    DeletePlayersFromTo(after_last_index, player_index);

    if (players_[player_index] == nullptr) {
      players_[player_index] = std::make_unique<Player>();
    }
    players_[player_index]->UpdateFromFromString(buffer);
    buffer += Player::LengthToSend();

    after_last_index = player_index + 1;
  }

  DeletePlayersFromTo(after_last_index, players_.size());

  players_.resize(after_last_index);
  ++number_of_changes_;
}

void ClientMap::DeletePlayersFromTo(unsigned int from, unsigned int to) {
  for (unsigned i = from; i < to; ++i) {
    players_[i] = nullptr;
  }
}

void ClientMap::UpdatePlayer(unsigned int player_id, char *buffer) {
  std::lock_guard lock(mutex_);
  if (players_.size() <= player_id) {
    players_.resize(player_id + 1);
  }
  if (players_[player_id] == nullptr) {
    players_[player_id] = std::make_unique<Player>();
  }
  players_[player_id]->UpdateFromFromString(buffer);
  ++number_of_changes_;
}

}  // namespace Client
