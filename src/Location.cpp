//
// Created by 2ToThe10th on 23.03.2020.
//

#include "Location.h"
Location::Location(unsigned x, unsigned y) : x_(x), y_(y) {}

Location::Location(const Location &location) {
  x_ = location.x_;
  y_ = location.y_;
}

unsigned Location::GetX() const {
  return x_;
}

unsigned Location::GetY() const {
  return y_;
}

void Location::SetX(unsigned x) {
  x_ = x;
}

void Location::SetY(unsigned y) {
  y_ = y;
}
