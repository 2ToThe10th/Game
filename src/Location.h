//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_LOCATION_H_
#define GAME_SRC_LOCATION_H_

class Location {
 public:
  Location() = default;

  Location(float x, float y);

  Location(const Location &location);

  [[nodiscard]] float GetX() const;

  [[nodiscard]] float GetY() const;

  void SetX(float x);

  void SetY(float y);

 private:
  float x_ = 0;
  float y_ = 0;
};

#endif //GAME_SRC_LOCATION_H_
