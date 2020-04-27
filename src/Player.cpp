//
// Created by 2ToThe10th on 23.03.2020.
//

#include <iostream>
#include <cstring>
#include "Player.h"


Player::Player(std::string name, const Location &start_location)
    : name_(std::move(name)), location_(start_location) {}

const Location &Player::GetLocation() const {
  return location_;
}

void Player::SetLocation(const Location &location) {
  location_ = location;
}

void Player::ToSend(char *buffer) {
  char is_command = false;
  memcpy(buffer, &is_command, sizeof(is_command));
  buffer += sizeof(is_command);
  float player_x = location_.GetX();
  memcpy(buffer, &player_x, sizeof(player_x));
  buffer += sizeof(player_x);
  float player_y = location_.GetY();
  memcpy(buffer, &player_y, sizeof(player_y));
  buffer += sizeof(player_y);
}

void Player::UpdateFromString(char *buffer) {
  float player_x;
  memcpy(&player_x, buffer, sizeof(player_x));
  buffer += sizeof(player_x);
  location_.SetX(player_x);
  float player_y;
  memcpy(&player_y, buffer, sizeof(player_y));
  buffer += sizeof(player_y);
  location_.SetY(player_y);
//  std::cout << "[UpdateFromString] " << player_x << "  " << player_y << std::endl;
}

void Player::UpdateFromPlayer(Player &player) {
  location_ = player.location_;
  if (name_ != player.name_) {
    name_ = player.name_;
  }
}

uint64_t Player::GetHashOfVector(std::vector<std::unique_ptr<Player>> &players) {
  uint64_t hash = 0;
  for (auto& player: players) {
    hash *= kMultiplierInVector;
    if (player != nullptr) {
      hash += player->GetPlayerHash();
    }
  }
  return hash;
}

uint64_t Player::GetPlayerHash() {
  uint64_t hash = 0;
  hash += location_.GetHash(kMultiplierInPlayer);
  return hash;
}

