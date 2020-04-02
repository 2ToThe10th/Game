//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_MARSHALING_H_
#define GAME_SRC_MARSHALING_H_

#include <string>
#include "Client/InputAgent/UserAction.h"


namespace Marshaling {

  std::string FromInputToUDPSocket(Client::InputAgent::UserAction user_input);

};

#endif //GAME_SRC_MARSHALING_H_
