//
// Created by 2ToThe10th on 23.03.2020.
//

#include "Player.h"
Player::Player(std::string name, const Location &start_location)
    : name_(std::move(name)), location_(start_location) {}

const Location &Player::GetLocation() const {
  return location_;
}

void Player::SetLocation(const Location &location) {
  location_ = location;
}
