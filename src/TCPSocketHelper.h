//
// Created by 2ToThe10th on 03.04.2020.
//

#ifndef GAME_SRC_TCPSOCKETHELPER_H_
#define GAME_SRC_TCPSOCKETHELPER_H_

#include <cstdlib>


namespace TCPSocketHelper {

class InetAtonException : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "inet aton";
  }
};

void WriteAll(int socket_fd, const char *buffer, size_t buffer_size);

void ReadAll(int socket_fd, char *buffer, size_t buffer_size);

void MakeNonblock(int socket_fd);

bool WouldBlock();

struct ConstBuffer {
 public:
  ConstBuffer() = delete;
  ConstBuffer(char *ptr, size_t size);

  char *GetBuffer();

  size_t GetSize();

  void WriteTo(int socket_fd) const;

  static ConstBuffer ReadFrom(int socket_fd);

  ~ConstBuffer();

 private:
  char *buffer_ = nullptr;
  size_t size_ = 0;
};

};

#endif //GAME_SRC_TCPSOCKETHELPER_H_
