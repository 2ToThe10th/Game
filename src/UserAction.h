//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_
#define GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_

#include <cstdlib>
#include "Angle.h"
#include "Command.h"


class UserAction {
 public:
  UserAction();

  [[nodiscard]] Angle GetAngle() const;
  [[nodiscard]] Command GetCommand() const;

  void SetAngle(Angle angle);

  [[nodiscard]] bool HasSomethingToSend() const;
  [[nodiscard]] bool IsCommandMessage() const;



  static UserAction Nothing();

  static UserAction UserCommand(Command command);

 private:
  bool is_command_message_ = false;
  Command command_;
  Angle angle_;
};

#endif //GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_
