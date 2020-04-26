//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_PLAYER_H_
#define GAME_SRC_PLAYER_H_

#include <string>

#include "Location.h"


class Player {
 public:
  explicit Player() = default;

  explicit Player(std::string name, const Location &start_location);

  [[nodiscard]] const Location &GetLocation() const;

  void SetLocation(const Location &location);

  constexpr static size_t LengthToSend() { // return Length that will be writen by ToSend()
    return sizeof(char) + 2 * sizeof(float);
  }

  void ToSend(char *buffer);

  void UpdateFromString(char *buffer);

  void UpdateFromPlayer(Player& player);

 private:
  Location location_;
  std::string name_;
 private:
};

#endif //GAME_SRC_PLAYER_H_
