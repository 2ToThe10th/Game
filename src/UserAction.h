//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_
#define GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_

#include <cstdlib>
#include "Angle.h"


class UserAction {
 public:
  UserAction();

  Angle GetAngle();

  void SetAngle(Angle angle);

  bool HasSomethingToSend();

  static UserAction Nothing();

 private:
  Angle angle_;
};

#endif //GAME_SRC_CLIENT_INPUTAGENT_USERACTION_H_
