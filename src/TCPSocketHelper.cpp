//
// Created by 2ToThe10th on 03.04.2020.
//

#include <unistd.h>
#include <system_error>
#include <fcntl.h>
#include <sys/socket.h>

#include "TCPSocketHelper.h"


namespace TCPSocketHelper {

void WriteAll(int socket_fd, const char *buffer, size_t buffer_size) {
  size_t already_written = 0;
  while (buffer_size > 0) {
    int was_written = send(socket_fd, buffer + already_written, buffer_size, MSG_NOSIGNAL);
    if (was_written < 0) {
      throw std::system_error(errno, std::generic_category());
    }
    already_written += was_written;
    buffer_size -= was_written;
  }
}

void ReadAll(int socket_fd, char *buffer, size_t buffer_size) {
  size_t already_read = 0;
  while (buffer_size > 0) {
    int was_read = read(socket_fd, buffer + already_read, buffer_size);
    if (was_read < 0) {
      throw std::system_error(errno, std::generic_category());
    }
    already_read += was_read;
    buffer_size -= was_read;
  }
}

void MakeNonblock(int socket_fd) {
  int flags = fcntl(socket_fd, F_GETFL);
  if (flags < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  flags |= O_NONBLOCK;
  if (fcntl(socket_fd, F_SETFL, flags) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

bool WouldBlock() {
  return errno == EWOULDBLOCK || errno == EAGAIN;
}

ConstBuffer::ConstBuffer(char *ptr, size_t size) : buffer_(ptr), size_(size) {

}

char *ConstBuffer::GetBuffer() {
  return buffer_;
}

size_t ConstBuffer::GetSize() {
  return size_;
}

void ConstBuffer::WriteTo(int socket_fd) const {
  WriteAll(socket_fd, reinterpret_cast<const char *>(&size_), sizeof(size_));
  WriteAll(socket_fd, buffer_, size_);
}

ConstBuffer ConstBuffer::ReadFrom(int socket_fd) {
  size_t size;
  ReadAll(socket_fd, reinterpret_cast<char *>(&size), sizeof(size));
  char *buffer = new char[size];
  ReadAll(socket_fd, buffer, size);
  return ConstBuffer(buffer, size);
}

ConstBuffer::~ConstBuffer() {
  if (size_ > 0) {
    delete[] buffer_;
  }
}

}