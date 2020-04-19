//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_
#define GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_

#include "../../Location.h"


namespace Server {

class PlayerState {
 public:
  PlayerState(unsigned player_id, Location new_location);

  unsigned GetPlayerId() const;

  Location GetNewLocation() const;
 private:
  unsigned player_id_;
  Location new_location_;
};

}
#endif //GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_
