//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ClientMap.h"
#include "../../Command.h"

#include <cstring>
#include <iostream>
#include <mutex>

namespace Client {

void ClientMap::UpdateByConstBuffer(
    TCPSocketHelper::ConstBuffer &const_buffer) { // TODO: normal handle error
  std::lock_guard lock(mutex_);

  unsigned handle_byte = UpdateExistPlayers(const_buffer);

  if (handle_byte < 0) {
    return;
  }

  char *current_buffer_position = const_buffer.GetBuffer() + handle_byte;
  char *end_of_buffer = const_buffer.GetEnd();

  unsigned one_row_len = sizeof(unsigned) + Player::LengthToSend();

  while (current_buffer_position + one_row_len <= end_of_buffer) {

    unsigned player_index = 0;
    memcpy(&player_index, current_buffer_position, sizeof(player_index));
    current_buffer_position += sizeof(player_index);

    if (player_index >= players_.size()) {
      players_.resize(player_index + 1);
    }

    if (players_[player_index] == nullptr) {
      players_[player_index] = std::make_unique<Player>();
    }
    players_[player_index]->UpdateFromString(current_buffer_position +
                                             sizeof(char));
    current_buffer_position += Player::LengthToSend();
  }

  ++number_of_changes_;
}

unsigned int ClientMap::UpdateExistPlayers(
    TCPSocketHelper::ConstBuffer
        &const_buffer) { // return how many bytes handle
  constexpr unsigned kBitInByte = 8;

  if (const_buffer.GetSize() < 1) {
    return -1;
  }

  uint8_t byte_with_players_num = 0;
  memcpy(&byte_with_players_num, const_buffer.GetBuffer(), sizeof(uint8_t));

  if (const_buffer.GetSize() < 1 + byte_with_players_num) {
    return -1;
  }

  for (unsigned iteration = 0; iteration < byte_with_players_num; ++iteration) {
    uint8_t exist_player = const_buffer.GetBuffer()[1 + iteration];
    uint8_t mask = 1;
    unsigned player_id = iteration * kBitInByte;
    for (unsigned bit = 0; bit < kBitInByte; ++bit) {
      if (exist_player & mask) {
        AddPlayerIfNotExist(player_id);
      } else {
        DeletePlayerIfExist(player_id);
      }
      mask *= 2;
      ++player_id;
    }
  }

  return 1 + byte_with_players_num;
}

void ClientMap::AddPlayerIfNotExist(unsigned int player_id) {
  if (player_id >= players_.size()) {
    players_.resize(player_id + 1);
  }
  if (!players_[player_id]) {
    players_[player_id] = std::make_unique<Player>();
  }
}

void ClientMap::DeletePlayerIfExist(unsigned int player_id) {
  if (player_id < players_.size() && players_[player_id]) {
    DeletePlayer(player_id);
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
      DeletePlayer(player_id);
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

void ClientMap::DeletePlayer(unsigned int player_id) {
  if (player_id >= players_.size()) {
    return;
  }
  ++number_of_changes_;
  players_[player_id] = nullptr;
}

TCPSocketHelper::ConstBuffer ClientMap::GetHash() {
  std::lock_guard lock(mutex_);
  auto tile_hash = Player::GetHashOfVector(players_, map_x_size_, map_y_size_);
  unsigned hash_array_size = sizeof(uint64_t) * tile_hash.size();
  char *hash_array = new char[hash_array_size];
  memcpy(hash_array, tile_hash.data(), hash_array_size);
  return TCPSocketHelper::ConstBuffer(hash_array, hash_array_size);
}

void ClientMap::SetMapSize(sf::Vector2u size) {
  map_x_size_ = size.x;
  map_y_size_ = size.y;
}

} // namespace Client
