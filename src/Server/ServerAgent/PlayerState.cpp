//
// Created by 2ToThe10th on 10.04.2020.
//

#include "PlayerState.h"


namespace Server {
PlayerState::PlayerState(unsigned player_id, const Location &new_location)
    : player_id_(player_id), new_location_(new_location) {
}

unsigned PlayerState::GetPlayerId() const {
  return player_id_;
}

Location PlayerState::GetNewLocation() const {
  return new_location_;
}

PlayerState::PlayerState(unsigned int player_id, Command command)
    : player_id_(player_id), command_(command) {

}

bool PlayerState::IsCommand() const {
  return command_ != Command::Nothing;
}

Command PlayerState::GetCommand() const {
  return command_;
}

}