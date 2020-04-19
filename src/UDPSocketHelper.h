//
// Created by 2ToThe10th on 04.04.2020.
//

#ifndef GAME_SRC_UDPSOCKETHELPER_H_
#define GAME_SRC_UDPSOCKETHELPER_H_

#include <cstddef>


namespace UDPSocketHelper {
void WriteAll(int socket_fd,
              const struct sockaddr_in &addr,
              const char *buffer,
              size_t buffer_size);
};

#endif //GAME_SRC_UDPSOCKETHELPER_H_
