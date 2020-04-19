//
// Created by 2ToThe10th on 10.04.2020.
//

#include "PlayerState.h"


namespace Server {
PlayerState::PlayerState(unsigned player_id, Location new_location)
    : player_id_(player_id), new_location_(new_location) {

}

unsigned PlayerState::GetPlayerId() const {
  return player_id_;
}

Location PlayerState::GetNewLocation() const {
  return new_location_;
}

}