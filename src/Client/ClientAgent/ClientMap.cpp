//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ClientMap.h"
#include "../../Command.h"

#include <iostream>
#include <mutex>
#include <cstring>


namespace Client {

void ClientMap::UpdateByConstBuffer(TCPSocketHelper::ConstBuffer &const_buffer) {
  std::lock_guard lock(mutex_);
  unsigned
      player_in_buffer_number = const_buffer.GetSize() / (sizeof(unsigned) + Player::LengthToSend());

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
    players_[player_index]->UpdateFromString(buffer + sizeof(char));
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
  char is_command = *buffer;
  buffer += sizeof(is_command);
  if (is_command) {
    unsigned unsigned_command;
    memcpy(&unsigned_command, buffer, sizeof(unsigned_command));
    buffer += sizeof(unsigned_command);
    auto command = static_cast<Command>(unsigned_command);
    if (command == Command::Disconnect) {
      DeleteOnePlayer(player_id);
    }
  } else {
    if (players_.size() <= player_id) {
      players_.resize(player_id + 1);
    }
    if (players_[player_id] == nullptr) {
      players_[player_id] = std::make_unique<Player>();
    }
    players_[player_id]->UpdateFromString(buffer);
    ++number_of_changes_;
  }
}

void ClientMap::DeleteOnePlayer(unsigned int player_id) {
  if (player_id >= players_.size()) {
    return;
  }
  ++number_of_changes_;
  players_[player_id] = nullptr;
}

uint64_t ClientMap::GetHash() {
  std::lock_guard lock(mutex_);
  return Player::GetHashOfVector(players_);
}

}  // namespace Client
