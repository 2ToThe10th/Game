//
// Created by 2ToThe10th on 10.04.2020.
//

#include "UserUpdate.h"


namespace Server {

UserUpdate::UserUpdate(unsigned player_id, UserAction user_action)
    : player_id_(player_id), user_action_(user_action) {}

unsigned UserUpdate::GetPlayerId() {
  return player_id_;
}

const UserAction &UserUpdate::GetUserAction() {
  return user_action_;
}
}