//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_
#define GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_

#include "../../Location.h"
#include "../../Command.h"


namespace Server {

class PlayerState {
 public:
  PlayerState(unsigned player_id, const Location& new_location);
  PlayerState(unsigned player_id, Command command);

  [[nodiscard]] bool IsCommand() const;

  [[nodiscard]] unsigned GetPlayerId() const;
  [[nodiscard]] Location GetNewLocation() const;
  [[nodiscard]] Command GetCommand() const;
 private:
  unsigned player_id_;
  Command command_ = Command::Nothing;
  Location new_location_;
};

}
#endif //GAME_SRC_SERVER_SERVERAGENT_PLAYERSTATE_H_
