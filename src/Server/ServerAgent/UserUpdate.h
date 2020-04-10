//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_SERVER_SERVERAGENT_USERUPDATE_H_
#define GAME_SRC_SERVER_SERVERAGENT_USERUPDATE_H_

#include "../../UserAction.h"


namespace Server {

class UserUpdate {
 public:
  UserUpdate(unsigned player_id, UserAction user_action);
  unsigned GetPlayerId();
  const UserAction& GetUserAction();

 private:
  unsigned player_id_;
  UserAction user_action_;
};

}
#endif //GAME_SRC_SERVER_SERVERAGENT_USERUPDATE_H_
