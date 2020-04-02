//
// Created by 2ToThe10th on 02.04.2020.
//

#include "Marshaling.h"


namespace Marshaling {

void AngleToString(Angle angle, std::string &action_string);

std::string FromInputToUDPSocket(Client::InputAgent::UserAction user_input) {

  std::string action_string;
  AngleToString(user_input.GetAngle(), action_string);

  return action_string;
}

void AngleToString(Angle angle, std::string &action_string) {
  action_string += char(angle);
}

}