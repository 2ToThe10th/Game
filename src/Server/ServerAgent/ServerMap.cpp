//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ServerMap.h"

#include <iostream>
#include <mutex>
#include <cstring>
#include <iomanip>


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

const sf::Image &ServerMap::GetImage() {
  return background_;
}

class AddPlayerException : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "could not load image";
  }
};

unsigned ServerMap::AddPlayer() {
  std::unique_lock lock(mutex_players_);
  if (number_of_players_ == players_.size()) {
    players_.push_back(new Player("player", Location(100, 100)));
    ++number_of_players_;
    return number_of_players_ - 1;
  } else {
    ++number_of_players_;
    for (size_t i = 0; i < players_.size(); ++i) {
      if (players_[i] == nullptr) {
        players_[i] = new Player("player", Location(100, 100));
        return i;
      }
    }
  }
  throw AddPlayerException();
}

void ServerMap::DeletePlayer(unsigned int player_id) {
  // TODO: udp notification
  std::unique_lock lock(mutex_players_);
  if (players_[player_id] != nullptr) {
    delete players_[player_id];
    players_[player_id] = nullptr;
  }
  while (!players_.empty() && players_.back() == nullptr) {
    players_.pop_back();
  }
  --number_of_players_;
}

TCPSocketHelper::ConstBuffer ServerMap::GetCurrentInfo() {
  std::unique_lock lock(mutex_players_);

  if (number_of_players_ == 0) {
    return TCPSocketHelper::ConstBuffer(nullptr, 0);
  }

  size_t buffer_size = number_of_players_ * (sizeof(unsigned) + 2 * sizeof(float));

  char *buffer = new char[buffer_size];

  int position_in_buffer = 0;

  for (unsigned i = 0; i < players_.size(); ++i) {
    if (players_[i] != nullptr) {
      memcpy(buffer + position_in_buffer, &i, sizeof(i));
      position_in_buffer += sizeof(i);
      players_[i]->ToSend(buffer + position_in_buffer);
      position_in_buffer += Player::LengthToSend();
    }
  }

  return TCPSocketHelper::ConstBuffer(buffer, buffer_size);
}

Location ServerMap::GetPlayerLocation(unsigned player_id) {
  std::unique_lock lock(mutex_players_);
  if (player_id < players_.size() && players_[player_id] != nullptr) {
    return players_[player_id]->GetLocation();
  } else {
    return Location();
  }
}

TCPSocketHelper::ConstBuffer ServerMap::SynchronizeAndPrepareSendString() {
  std::unique_lock lock(mutex_players_);
  auto queue = physics_to_map_queue_.GetQueue();

  size_t length_to_send = (sizeof(unsigned) + Player::LengthToSend()) * queue.size();
  char* to_send = new char[length_to_send];
  char* current_position_in_to_send = to_send;
  while (!queue.empty()) {
    auto player_state = queue.front();
    unsigned player_id = player_state.GetPlayerId();
    queue.pop();
    players_[player_id]->SetLocation(player_state.GetNewLocation());
    memcpy(current_position_in_to_send, &player_id, sizeof(player_id));
    current_position_in_to_send += sizeof(player_id);
    players_[player_id]->ToSend(current_position_in_to_send);
    current_position_in_to_send += Player::LengthToSend();
  }

  return TCPSocketHelper::ConstBuffer(to_send, length_to_send);
}

}  // namespace Server
