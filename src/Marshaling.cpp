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

//  std::cout << static_cast<int>(user_action.GetAngle()) << " " << static_cast<int>(user_action.GetCommand()) << " " << user_action.IsCommandMessage() << std::endl;

  std::string action_string;

  if (user_action.IsCommandMessage()) {
//    std::cout << "[FromInputToUDPMessage] Command" << std::endl;
    action_string += kCommandStart;
    action_string += static_cast<char>(user_action.GetCommand());
  } else {
    AngleToString(user_action.GetAngle(), action_string);
  }
//  std::cout << action_string.size() << std::endl;
  return action_string;
}

UserAction FromUDPMessageToUserAction(const char *buffer, size_t buffer_size) {
  if (buffer_size < 1) {
    return UserAction::Nothing();
  }
  UserAction user_action;
//  std::cout << buffer_size << std::endl;
  if (buffer[0] == kCommandStart) {
    if (buffer_size < 2) {
      return UserAction::Nothing();
    } else {
//      std::cout << "[FromUDPMessageToUserAction] Received command: " << static_cast<int>(buffer[1]) << std::endl;
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