//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_PLAYER_H_
#define GAME_SRC_PLAYER_H_

#include <string>

#include "Location.h"


class Player {
 public:
  explicit Player(std::string name, const Location &start_location);

  [[nodiscard]] const Location &GetLocation() const;

  void SetLocation(const Location &location);

 private:
  Location location_;
  std::string name_;
  bool is_valid;
 private:
};

#endif //GAME_SRC_PLAYER_H_
