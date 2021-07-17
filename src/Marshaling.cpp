//
// Created by 2ToThe10th on 02.04.2020.
//

#include <cassert>
#include <iostream>
#include "Marshaling.h"


namespace Marshaling {

constexpr char kCommandStart = static_cast<char>(127);

void AngleToString(Angle angle, std::string &action_string);

std::string FromInputToUDPMessage(const UserAction& user_action) {


  std::string action_string;

  if (user_action.IsCommandMessage()) {
    action_string += kCommandStart;
    action_string += static_cast<char>(user_action.GetCommand());
  } else {
    AngleToString(user_action.GetAngle(), action_string);
  }
  return action_string;
}

UserAction FromUDPMessageToUserAction(const char *buffer, size_t buffer_size) {
  if (buffer_size < 1) {
    return UserAction::Nothing();
  }
  UserAction user_action;
  if (buffer[0] == kCommandStart) {
    if (buffer_size < 2) {
      return UserAction::Nothing();
    } else {
      return UserAction::UserCommand(static_cast<Command>(buffer[1]));
    }
  } else {
    user_action.SetAngle(Angle(buffer[0]));
    return user_action;
  }
}

void AngleToString(Angle angle, std::string &action_string) {
  action_string += char(angle);
}

}