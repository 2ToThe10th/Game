//
// Created by 2ToThe10th on 02.04.2020.
//

#include <utility>
#include "UserAction.h"


UserAction::UserAction() {
  angle_ = Angle::None;
}

Angle UserAction::GetAngle() const {
  return angle_;
}

void UserAction::SetAngle(Angle angle) {
  angle_ = angle;
}

bool UserAction::HasSomethingToSend() const {
  return angle_ != Angle::None;
}

UserAction UserAction::Nothing() {
  UserAction user_action;
  return user_action;
}

UserAction UserAction::UserCommand(Command command) {
  UserAction user_action;
  user_action.is_command_message_ = true;
  user_action.command_ = command;
  return user_action;
}

bool UserAction::IsCommandMessage() const {
  return is_command_message_;
}

Command UserAction::GetCommand() const {
  return command_;
}

