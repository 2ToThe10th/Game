//
// Created by 2ToThe10th on 23.03.2020.
//

#include "Location.h"


Location::Location(float x, float y) : x_(x), y_(y) {}

Location::Location(const Location &location) {
  x_ = location.x_;
  y_ = location.y_;
}

float Location::GetX() const {
  return x_;
}

float Location::GetY() const {
  return y_;
}

void Location::SetX(float x) {
  x_ = x;
}

void Location::SetY(float y) {
  y_ = y;
}

