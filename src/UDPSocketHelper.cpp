//
// Created by 2ToThe10th on 04.04.2020.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>

#include "UDPSocketHelper.h"


namespace UDPSocketHelper {

void WriteAll(int socket_fd,
              const struct sockaddr_in &addr,
              const char *buffer,
              size_t buffer_size) {
  size_t already_written = 0;
  while (buffer_size > 0) {
    int was_written = sendto(socket_fd, buffer + already_written, buffer_size, MSG_NOSIGNAL,
                             reinterpret_cast<const sockaddr *>(&addr),
                             sizeof(addr));
    if (was_written < 0) {
      throw std::system_error(errno, std::generic_category());
    }
    already_written += was_written;
    buffer_size -= was_written;
  }
}

}