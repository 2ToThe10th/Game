//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_LOCATION_H_
#define GAME_SRC_LOCATION_H_

#include <cstdint>
#include <cstring>


class Location {
 public:
  Location() = default;

  Location(float x, float y);

  Location(const Location &location);

  [[nodiscard]] float GetX() const;

  [[nodiscard]] float GetY() const;

  void SetX(float x);

  void SetY(float y);

  inline uint64_t GetHash(uint64_t multiplier);

 private:
  float x_ = 0;
  float y_ = 0;
};

uint64_t Location::GetHash(uint64_t multiplier) {
  uint64_t x_to_uint64_t = 0;
  memcpy(&x_to_uint64_t, &x_, sizeof(x_));
  uint64_t y_to_uint64_t = 0;
  memcpy(&y_to_uint64_t, &y_, sizeof(y_));
  return x_to_uint64_t * multiplier + y_to_uint64_t;
}

#endif //GAME_SRC_LOCATION_H_
