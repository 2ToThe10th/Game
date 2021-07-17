//
// Created by 2ToThe10th on 23.03.2020.
//

#include "Player.h"
#include <cmath>
#include <cstring>
#include <iostream>

Player::Player(std::string name, const Location &start_location)
    : name_(std::move(name)), location_(start_location) {}

const Location &Player::GetLocation() const { return location_; }

void Player::SetLocation(const Location &location) { location_ = location; }

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
}

void Player::UpdateFromPlayer(Player &player) {
  location_ = player.location_;
  if (name_ != player.name_) {
    name_ = player.name_;
  }
}

uint64_t Player::GetPlayerHash() {
  uint64_t hash = 0;
  hash += location_.GetHash(kMultiplierInPlayer);
  return hash;
}

inline unsigned Player::NumberOfTile(const unsigned map_size,
                                     const unsigned tile_size) {
  return (map_size - 1) / tile_size + 1;
}

inline unsigned Player::PlaceInHashVector(const Location &location,
                                          const unsigned int number_of_x_tile) {
  unsigned x_tile = std::floor(location.GetX() / static_cast<float>(kTileXSize));
  unsigned y_tile = std::floor(location.GetY() / static_cast<float>(kTileYSize));

  return number_of_x_tile * y_tile + x_tile + 1; // 1 for first hash for all
}

std::vector<uint64_t>
Player::GetHashOfVector(std::vector<std::unique_ptr<Player>> &players,
                        unsigned int map_x_size, unsigned int map_y_size) {
  unsigned number_of_x_tile = NumberOfTile(map_x_size, kTileXSize);
  unsigned number_of_y_tile = NumberOfTile(map_y_size, kTileYSize);

  std::vector<uint64_t> hash_vector(number_of_x_tile * number_of_y_tile +
                                    1); // 1 for full hash

  for (auto &player : players) {
    hash_vector[0] *= kMultiplierInVector;
    if (player != nullptr) {
      uint64_t hash = player->GetPlayerHash();
      hash_vector[0] += hash;
      unsigned hash_to_add = PlaceInHashVector(
          player->GetLocation(), number_of_x_tile);
      hash_vector[hash_to_add] *= kMultiplierInVector;
      hash_vector[hash_to_add] += hash;
    }
  }

  return std::move(hash_vector);
}
unsigned Player::NumberOfXTile(unsigned int map_x_size) {
  return NumberOfTile(map_x_size, kTileXSize);
}
