//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ServerMap.h"

#include <iostream>
#include <mutex>
#include <cstring>


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
  std::unique_lock lock(mutex_);
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
  std::unique_lock lock(mutex_);
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
  std::unique_lock lock(mutex_);
  // TODO: Update from queue

  if (number_of_players_ == 0) {
    return TCPSocketHelper::ConstBuffer(nullptr, 0);
  }

  size_t buffer_size = number_of_players_ * (sizeof(unsigned) + 2 * sizeof(float));

  char *buffer = new char[buffer_size];

  int position_in_buffer = 0;

  for (unsigned i = 0; i < players_.size(); ++i) {
    std::cout << i << "  ";
    if (players_[i] == nullptr) {
      std::cout << "nullptr";
    } else {
      std::cout << players_[i]->GetLocation().GetX() << " "
                << players_[i]->GetLocation().GetY();
    }
    std::cout << std::endl;
    if (players_[i] != nullptr) {
      memcpy(buffer + position_in_buffer, &i, sizeof(i));
      position_in_buffer += sizeof(i);
      players_[i]->ToSend(buffer + position_in_buffer);
      position_in_buffer += Player::LengthToSend();
    }
  }

  return TCPSocketHelper::ConstBuffer(buffer, buffer_size);
}

void ServerMap::SetPlayerLocation(unsigned player_id, Location new_location) {
  std::unique_lock lock(mutex_);
  if (player_id < players_.size() && players_[player_id] != nullptr) {
    players_[player_id]->SetLocation(new_location);
  }
}

Location ServerMap::GetPlayerLocation(unsigned player_id) {
  std::unique_lock lock(mutex_);
  if (player_id < players_.size() && players_[player_id] != nullptr) {
    return players_[player_id]->GetLocation();
  } else {
    return Location();
  }
}

}  // namespace Server
