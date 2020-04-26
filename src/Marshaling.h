//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_MARSHALING_H_
#define GAME_SRC_MARSHALING_H_

#include <string>
#include "UserAction.h"


namespace Marshaling {

std::string FromInputToUDPMessage(const UserAction& user_action);

UserAction FromUDPMessageToUserAction(const char *buffer, size_t buffer_size);

};

#endif //GAME_SRC_MARSHALING_H_
