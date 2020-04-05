//
// Created by 2ToThe10th on 02.04.2020.
//

#include <cassert>
#include <iostream>
#include "Marshaling.h"


namespace Marshaling {

void AngleToString(Angle angle, std::string &action_string);

std::string FromInputToUDPMessage(UserAction user_input) {

  std::string action_string;
  AngleToString(user_input.GetAngle(), action_string);

  return action_string;
}

UserAction FromUDPMessageToUserAction(const char *buffer, size_t buffer_size) {
  assert(buffer_size == 1);
  UserAction user_action;
  user_action.SetAngle(Angle(buffer[0]));
  return user_action;
}

void AngleToString(Angle angle, std::string &action_string) {
  action_string += char(angle);
}

}