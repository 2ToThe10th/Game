//
// Created by 2ToThe10th on 06.04.2020.
//

#include "EpollOneReturn.h"
#include <sys/epoll.h>
#include <system_error>
#include <unistd.h>


EpollOneReturn::EpollOneReturn() {
  epoll_fd_ = epoll_create(1);
  if (epoll_fd_ < 1) {
    throw std::system_error(errno, std::generic_category());
  }
}

void EpollOneReturn::Add(int socket, void *data) {
  struct epoll_event event{};
  event.events = EPOLLIN;
  event.data.ptr = data;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket, &event) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

bool EpollOneReturn::Wait(int timeout_millisecond) {
  struct epoll_event epoll_event{};
  return epoll_wait(epoll_fd_, &epoll_event, 1, timeout_millisecond) > 0;
}

EpollOneReturn::~EpollOneReturn() {
  close(epoll_fd_);
}