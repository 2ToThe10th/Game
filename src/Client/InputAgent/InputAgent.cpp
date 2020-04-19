//
// Created by 2ToThe10th on 02.04.2020.
//

#include "InputAgent.h"


namespace Client::InputAgent {

InputAgent::InputAgent() : last_send_info_time_(Time::Now()) {

}

void InputAgent::Initialize(sf::RenderWindow &window) const {
  window.setKeyRepeatEnabled(false);
}

void InputAgent::HandleEvent(sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    HandlerKeyPressed(event);
  }
  if (event.type == sf::Event::KeyReleased) {
    HandlerKeyRelease(event);
  }
}

void InputAgent::HandlerKeyPressed(sf::Event &event) {
  key_pressed_[event.key.code] = true;
}

void InputAgent::HandlerKeyRelease(sf::Event &event) {
  key_pressed_[event.key.code] = false;
}

UserAction InputAgent::GetUserAction() const {
  if (Time::Now() - last_send_info_time_ < kSendInterval) {
    return UserAction::Nothing();
  }

  UserAction user_action;
  if (IsUpKeyPressed() && !IsDownKeyPressed()) {
    user_action.SetAngle(ChooseAngle(Angle::UpLeft, Angle::Up, Angle::UpRight));
  } else if (!IsUpKeyPressed() && IsDownKeyPressed()) {
    user_action.SetAngle(ChooseAngle(Angle::DownLeft, Angle::Down, Angle::DownRight));
  } else {
    user_action.SetAngle(ChooseAngle(Angle::Left, Angle::None, Angle::Right));
  }

  return user_action;
}

Angle InputAgent::ChooseAngle(Angle angle_left, Angle angle_none, Angle angle_right) const {
  if (IsLeftKeyPressed() && !IsRightKeyPressed()) {
    return angle_left;
  } else if (!IsLeftKeyPressed() && IsRightKeyPressed()) {
    return angle_right;
  } else {
    return angle_none;
  }
}

bool InputAgent::IsUpKeyPressed() const {
  return key_pressed_[sf::Keyboard::W];
}

bool InputAgent::IsDownKeyPressed() const {
  return key_pressed_[sf::Keyboard::S];
}

bool InputAgent::IsRightKeyPressed() const {
  return key_pressed_[sf::Keyboard::D];
}

bool InputAgent::IsLeftKeyPressed() const {
  return key_pressed_[sf::Keyboard::A];
}

}