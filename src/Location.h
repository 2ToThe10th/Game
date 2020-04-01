//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_LOCATION_H_
#define GAME_SRC_LOCATION_H_

class Location {
 public:
  Location() = default;

  Location(unsigned x, unsigned y);

  Location(const Location &location);

  [[nodiscard]] unsigned GetX() const;

  [[nodiscard]] unsigned GetY() const;

  void SetX(unsigned x);

  void SetY(unsigned y);

 private:
  unsigned x_ = 0;
  unsigned y_ = 0;
};

#endif //GAME_SRC_LOCATION_H_
