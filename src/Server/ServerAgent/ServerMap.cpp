//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ServerMap.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace Server {

class ImageLoadException : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "could not load image";
  }
};

void ServerMap::LoadImageFromFile(const std::string &image_file) {
  if (!background_.loadFromFile(image_file)) {
    throw ImageLoadException();
  }
}

const sf::Image &ServerMap::GetImage() { return background_; }

class AddPlayerException : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "could not load image";
  }
};

unsigned ServerMap::AddPlayer() {
  std::lock_guard lock(mutex_players_);
  if (number_of_players_ == players_.size()) {
    players_.push_back(std::make_unique<Player>("player", Location(100, 100)));
    ++number_of_players_;
    return number_of_players_ - 1;
  } else {
    ++number_of_players_;
    for (size_t i = 0; i < players_.size(); ++i) {
      if (players_[i] == nullptr) {
        players_[i] = std::make_unique<Player>("player", Location(100, 100));
        return i;
      }
    }
  }
  throw AddPlayerException();
}

void ServerMap::DeletePlayer(unsigned int player_id, bool is_already_lock) {
  if (!is_already_lock) {
    mutex_players_.lock();
  }
  if (player_id < players_.size() && players_[player_id] != nullptr) {
    players_[player_id] = nullptr;
    --number_of_players_;
  }
  while (!players_.empty() && players_.back() == nullptr) {
    players_.pop_back();
  }
  if (!is_already_lock) {
    mutex_players_.unlock();
  }
}

TCPSocketHelper::ConstBuffer
ServerMap::GetDifference(std::vector<uint64_t> &client_hash) {
  std::lock_guard lock(mutex_players_);

  auto exist_players = GetExistPlayer();

  auto current_hash = GetHashWithoutMutex();

  std::vector<unsigned> player_to_send;

  for (unsigned i = 0; i < players_.size(); ++i) {
    if (players_[i] != nullptr) {
      unsigned place_in_hash_vector = Player::PlaceInHashVector(
          players_[i]->GetLocation(),
          Player::NumberOfXTile(background_.getSize().x));
      if (client_hash[place_in_hash_vector] !=
          current_hash[place_in_hash_vector]) {
        player_to_send.emplace_back(i);
      }
    }
  }

  size_t player_buffer_size =
      player_to_send.size() * (sizeof(unsigned) + Player::LengthToSend());

  char *player_buffer = new char[player_buffer_size];

  int position_in_buffer = 0;

  for (auto player_id : player_to_send) {
    memcpy(player_buffer + position_in_buffer, &player_id, sizeof(player_id));
    position_in_buffer += sizeof(player_id);
    players_[player_id]->ToSend(player_buffer + position_in_buffer);
    position_in_buffer += Player::LengthToSend();
  }

  char *buffer = new char[exist_players.GetSize() + player_buffer_size];

  memmove(buffer, exist_players.GetBuffer(), exist_players.GetSize());
  memmove(buffer + exist_players.GetSize(), player_buffer, player_buffer_size);

  delete[] player_buffer;

  return TCPSocketHelper::ConstBuffer(buffer, exist_players.GetSize() +
                                                  player_buffer_size);
}

TCPSocketHelper::ConstBuffer ServerMap::GetExistPlayer() {
  constexpr unsigned kBitInByte = 8;

  const uint8_t byte_to_send_players_num =
      ((players_.size() - 1) / kBitInByte) + 1;

  char *send_exist_player =
      new char[sizeof(byte_to_send_players_num) + byte_to_send_players_num];

  memcpy(send_exist_player, &byte_to_send_players_num,
         sizeof(byte_to_send_players_num));

  for (unsigned iteration = 0; iteration < byte_to_send_players_num;
       ++iteration) {
    uint8_t exist_player = 0;
    uint8_t mask = 1;
    unsigned player_id = iteration * kBitInByte;
    for (unsigned bit = 0; bit < kBitInByte; ++bit) {
      if (player_id < players_.size()) {
        if (players_[player_id]) { // operator bool of unique_ptr return true if
          // object exist
          exist_player |= mask;
        }
        mask *= 2;
        ++player_id;
      } else {
        break;
      }
    }

    memmove(send_exist_player + sizeof(byte_to_send_players_num) + iteration,
            &exist_player, sizeof(exist_player));
  }

  return TCPSocketHelper::ConstBuffer(send_exist_player,
                                      sizeof(byte_to_send_players_num) +
                                          byte_to_send_players_num);
}

Location ServerMap::GetPlayerLocation(unsigned player_id) {
  std::lock_guard lock(mutex_players_);
  if (player_id < players_.size() && players_[player_id] != nullptr) {
    return players_[player_id]->GetLocation();
  } else {
    return Location();
  }
}

TCPSocketHelper::ConstBuffer ServerMap::SynchronizeAndPrepareSendString() {
  std::lock_guard lock(mutex_players_);
  was_synchronized_ = true;
  auto queue = physics_to_map_queue_.GetQueue();

  size_t length_to_send =
      (sizeof(unsigned) + Player::LengthToSend()) * queue.size();
  char *to_send = new char[length_to_send];
  char *current_position_in_to_send = to_send;
  while (!queue.empty()) {
    auto player_state = queue.front();
    unsigned player_id = player_state.GetPlayerId();
    queue.pop();
    if (player_state.IsCommand()) {
      if (player_state.GetCommand() == Command::Disconnect) {
        HandleDeletedPlayer(player_id, current_position_in_to_send);
        current_position_in_to_send +=
            sizeof(player_id) + Player::LengthToSend();
      }
    } else {
      players_[player_id]->SetLocation(player_state.GetNewLocation());
      memcpy(current_position_in_to_send, &player_id, sizeof(player_id));
      current_position_in_to_send += sizeof(player_id);
      players_[player_id]->ToSend(current_position_in_to_send);
      current_position_in_to_send += Player::LengthToSend();
    }
  }

  return TCPSocketHelper::ConstBuffer(to_send, length_to_send);
}

size_t ServerMap::NumberOfPlayers() {
  std::lock_guard lock(mutex_players_);
  return players_.size();
}

bool ServerMap::WasSynchronized() {
  std::lock_guard lock(mutex_players_);
  if (was_synchronized_) {
    was_synchronized_ = false;
    return true;
  } else {
    return false;
  }
}

void ServerMap::HandleDeletedPlayer(unsigned int player_id,
                                    char *current_position_in_buffer) {
  DeletePlayer(player_id, true);
  memcpy(current_position_in_buffer, &player_id, sizeof(player_id));
  current_position_in_buffer += sizeof(player_id);
  char is_command = true;
  memcpy(current_position_in_buffer, &is_command, sizeof(is_command));
  current_position_in_buffer += sizeof(is_command);
  auto command = static_cast<unsigned>(Command::Disconnect);
  memcpy(current_position_in_buffer, &command, sizeof(command));
  current_position_in_buffer += sizeof(command);
}

std::vector<uint64_t> ServerMap::GetHashWithoutMutex() {
  return Player::GetHashOfVector(players_, background_.getSize().x,
                                 background_.getSize().y);
}
std::vector<uint64_t> ServerMap::GetHash() {
  std::lock_guard lock(mutex_players_);
  return GetHashWithoutMutex();
}

} // namespace Server
