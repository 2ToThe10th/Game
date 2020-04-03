//
// Created by 2ToThe10th on 02.04.2020.
//

#include "UserAction.h"


UserAction::UserAction() {
  angle_ = Angle::None;
}

Angle UserAction::GetAngle() {
  return angle_;
}

void UserAction::SetAngle(Angle angle) {
  angle_ = angle;
}

bool UserAction::HasSomethingToSend() {
  return angle_ != Angle::None;
}

UserAction UserAction::Nothing() {
  UserAction user_action;
  return user_action;
}
