//
// Created by 2ToThe10th on 06.04.2020.
//

#ifndef GAME_SRC_EPOLLONERETURN_H_
#define GAME_SRC_EPOLLONERETURN_H_

class EpollOneReturn {
 public:
  EpollOneReturn();

  void Add(int socket);

  bool Wait(int timeout_millisecond);

  ~EpollOneReturn();
 private:
  int epoll_fd_;
};

#endif //GAME_SRC_EPOLLONERETURN_H_
