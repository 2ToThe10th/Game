//
// Created by 2ToThe10th on 23.03.2020.
//

#include <iostream>
#include "GameGraphic.h"
#include "Angel.h"
#include "ClientMap.h"

GameGraphic::GameGraphic(ClientMap &map, sf::Window &window)
    : map_(map), window_(window), last_send_info_time_(Now()) {
  last_send_info_time_ = Now();
}

void GameGraphic::GameLoop() {
  window_.setKeyRepeatEnabled(false);
  while (window_.isOpen()) {
    sf::Event event{};
    while (window_.pollEvent(event)) {
      HandlerEvent(event);
    }
    WatchUserAction();

    if (map_.IsChanged()) {
      Draw();
    }
  }
}

void GameGraphic::HandlerEvent(sf::Event &event) {
  if (event.type == sf::Event::Closed) {
    window_.close();
  }
  if (event.type == sf::Event::KeyPressed) {
    HandlerKeyPressed(event);
  }
  if (event.type == sf::Event::KeyReleased) {
    HandlerKeyRelease(event);
  }
}

void GameGraphic::HandlerKeyPressed(sf::Event &event) {
  switch (event.key.code) {
    case sf::Keyboard::W:up_key_pressed_ = true;
      break;
    case sf::Keyboard::A:left_key_pressed_ = true;
      break;
    case sf::Keyboard::S:down_key_pressed_ = true;
      break;
    case sf::Keyboard::D:right_key_pressed_ = true;
      break;
    default:break;
  }
}

void GameGraphic::HandlerKeyRelease(sf::Event &event) {
  switch (event.key.code) {
    case sf::Keyboard::W:up_key_pressed_ = false;
      break;
    case sf::Keyboard::A:left_key_pressed_ = false;
      break;
    case sf::Keyboard::S:down_key_pressed_ = false;
      break;
    case sf::Keyboard::D:right_key_pressed_ = false;
      break;
    default:break;
  }
}

void GameGraphic::WatchUserAction() {
  if (Now() - last_send_info_time_ < send_interval) {
    return;
  }

  Angle angle;
  if ((!up_key_pressed_ && !down_key_pressed_)
      || (up_key_pressed_ && down_key_pressed_)) {
    angle = ChooseAngle(Angle::Left, Angle::None, Angle::Right);
  } else {
    if (up_key_pressed_) {
      angle = ChooseAngle(Angle::UpLeft, Angle::Up, Angle::UpRight);
    } else {
      angle =
          ChooseAngle(Angle::DownLeft, Angle::Down, Angle::DownRight);
    }
  }

  if (angle != Angle::None) {
    SendUserAction(angle);
    std::cout << (unsigned) angle << std::endl;
  }
}

Angle GameGraphic::ChooseAngle(Angle angle_left,
                               Angle angle_none,
                               Angle angle_right) const {
  if (left_key_pressed_ && !right_key_pressed_) {
    return angle_left;
  } else if (!left_key_pressed_ && right_key_pressed_) {
    return angle_right;
  } else {
    return angle_none;
  }
}

Duration GameGraphic::Now() const {
  return std::chrono::duration_cast<Duration>
      (std::chrono::steady_clock::now().time_since_epoch());
}

void GameGraphic::SendUserAction(Angle angle) {
  // Udp Socket . send angle in pretty view
}

void GameGraphic::Draw() {

}
