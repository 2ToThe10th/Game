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
  std::unique_lock lock(mutex_);
  size_t player_in_buffer_number = const_buffer.GetSize() / (3 * sizeof(unsigned));

  char *buffer = const_buffer.GetBuffer();
  size_t place_in_buffer = 0;

  size_t after_last_index = 0;

  for (size_t i = 0; i < player_in_buffer_number; ++i) {
    unsigned player_index = 0; //TODO: make class and function for this lines
    unsigned player_x = 0;
    unsigned player_y = 0;

    memcpy(&player_index, buffer + place_in_buffer, sizeof(player_index));
    place_in_buffer += sizeof(player_index);
    memcpy(&player_x, buffer + place_in_buffer, sizeof(player_x));
    place_in_buffer += sizeof(player_x);
    memcpy(&player_y, buffer + place_in_buffer, sizeof(player_y));
    place_in_buffer += sizeof(player_y);

    if (player_index >= players_.size()) {
      players_.resize(player_index + 1, nullptr);
    }

    DeletePlayersFromTo(after_last_index, player_index);

    if (players_[player_index] == nullptr) {
      players_[player_index] = new Player("player", Location(player_x, player_y));
    } else {
      players_[player_index]->SetLocation(Location(player_x, player_y));
    }
    after_last_index = player_index + 1;
  }

  DeletePlayersFromTo(after_last_index, players_.size());

  players_.resize(after_last_index, nullptr);
}

void ClientMap::DeletePlayersFromTo(size_t from, size_t to) {
  for (size_t i = from; i < to; ++i) {
    if (players_[i] != nullptr) {
      delete players_[i];
      players_[i] = nullptr;
    }
  }
}
}  // namespace Client
