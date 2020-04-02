//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_TCPSOCKETHELPER_H_
#define GAME_SRC_TCPSOCKETHELPER_H_

#include <cstdlib>


namespace TCPSocketHelper {

void WriteAll(int socket_fd, char *buffer, size_t buffer_size);

};

#endif //GAME_SRC_TCPSOCKETHELPER_H_
