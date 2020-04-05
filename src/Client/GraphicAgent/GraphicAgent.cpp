//
// Created by 2ToThe10th on 23.03.2020.
//

#include <iostream>
#include "GraphicAgent.h"
#include "../../Angle.h"


namespace Client::GraphicAgent {

GraphicAgent::GraphicAgent(ClientMap &main_map, sf::RenderWindow &window)
    : graphic_map_(main_map), window_(window) {

}

void GraphicAgent::Initialize(sf::Image &&image) {
  texture_.loadFromImage(image);
  font_.loadFromFile("../res/ArialMT.ttf");
}

void GraphicAgent::Draw() {
  window_.clear(sf::Color::White);

  window_.draw(sf::Sprite(texture_));

  std::string current_situation;

  int i = 0;
  for (auto player: graphic_map_.main_map_.players_) {
    current_situation += "index = ";
    current_situation += std::to_string(i);
    if (player == nullptr) {
      current_situation += "   nullptr\n";
    } else {
      current_situation += " Location = ";
      current_situation += std::to_string(player->GetLocation().GetX());
      current_situation += ", ";
      current_situation += std::to_string(player->GetLocation().GetY());
      current_situation += "\n";
    }
    ++i;
  }

  sf::Text text(current_situation, font_);
  text.setFillColor(sf::Color::Black);

  window_.draw(text);

  window_.display();
}

}



/*
Location place(0, 0);

GraphicAgent::GraphicAgent(ServerMap &map, sf::RenderWindow &window)
    : map_(map), window_(window), last_send_info_time_(Now()) {
  texture_.loadFromImage(map_.GetImage());
}

void GraphicAgent::GameLoop() {
  window_.setKeyRepeatEnabled(false);
  while (window_.isOpen()) {
    sf::Event event{};
    while (window_.pollEvent(event)) {
      HandlerEvent(event);
    }
    WatchUserAction();

    if (map_.IsChanged() || 1) {//TODO:delete 1
      Draw();
    }
  }
}

void GraphicAgent::HandlerEvent(sf::Event &event) {
  if (event.type == sf::Event::Closed) {
    window_.close();
  }
  if (event.type == sf::Event::KeyPressed) {
    HandlerKeyPressed(event);
  }
  if (event.type == sf::Event::KeyReleased) {
    HandlerKeyRelease(event);
  }
  if (event.type == sf::Event::Resized) {
    //TODO: Changed
  }
}

void GraphicAgent::HandlerKeyPressed(sf::Event &event) {
  key_pressed_[event.key.code] = true;
}

void GraphicAgent::HandlerKeyRelease(sf::Event &event) {
  key_pressed_[event.key.code] = false;
}

// proxy class - marshaling

void GraphicAgent::WatchUserAction() {
  if (Now() - last_send_info_time_ < kSendInterval) {
    return;
  }

  Angle angle;
  if (IsUpKeyPressed() && !IsDownKeyPressed()) {
    angle = ChooseAngle(Angle::UpLeft, Angle::Up, Angle::UpRight);
  } else if (!IsUpKeyPressed() && IsDownKeyPressed()) {
    angle = ChooseAngle(Angle::DownLeft, Angle::Down, Angle::DownRight);
  } else {
    angle = ChooseAngle(Angle::Left, Angle::None, Angle::Right);
  }

  if (angle != Angle::None) {
    SendUserAction(angle);
    std::cout << (unsigned) angle << std::endl;
  }
}

Angle GraphicAgent::ChooseAngle(Angle angle_left, Angle angle_none, Angle angle_right) const {
  if (IsLeftKeyPressed() && !IsRightKeyPressed()) {
    return angle_left;
  } else if (!IsLeftKeyPressed() && IsRightKeyPressed()) {
    return angle_right;
  } else {
    return angle_none;
  }
}

bool GraphicAgent::IsUpKeyPressed() const {
  return key_pressed_[sf::Keyboard::W];
}

bool GraphicAgent::IsDownKeyPressed() const {
  return key_pressed_[sf::Keyboard::S];
}

bool GraphicAgent::IsRightKeyPressed() const {
  return key_pressed_[sf::Keyboard::D];
}

bool GraphicAgent::IsLeftKeyPressed() const {
  return key_pressed_[sf::Keyboard::A];
}

void GraphicAgent::SendUserAction(Angle angle) {
  if (angle == Angle::Up) {
    place.SetY(std::max<unsigned>(place.GetY(), 1) - 1);
  }
  if (angle == Angle::Down) {
    place.SetY(place.GetY() + 1);
  }
  if (angle == Angle::Left) {
    place.SetX(std::max<unsigned>(place.GetX(), 1) - 1);
  }
  if (angle == Angle::Right) {
    place.SetX(place.GetX() + 1);
  }
  // Udp Socket . send angle in pretty view
}

void GraphicAgent::Draw() {
  sf::Sprite sprite(texture_);

  sprite.setTextureRect(sf::IntRect(
      std::max(place.GetX(), window_.getSize().x / 2) - window_.getSize().x / 2,
      std::max(place.GetY(), window_.getSize().y / 2) - window_.getSize().y / 2,
      texture_.getSize().x,
      texture_.getSize().y));// TODO: make normal

  sprite.setScale((float) texture_.getSize().x / window_.getSize().x,
                  (float) texture_.getSize().y / window_.getSize().y);

  std::cout << "Draw   " << sprite.getGlobalBounds().top << "  " << sprite.getGlobalBounds().height
            << "  "
            << sprite.getGlobalBounds().left << "  " << sprite.getGlobalBounds().width << "   "
            << window_.getSize().x
            << "  " << window_.getSize().y << std::endl;

  window_.clear(sf::Color::Black);

  window_.draw(sprite);

  window_.display();

}
GraphicAgent::GraphicAgent(sf::RenderWindow &window, ServerMap &main_map) : graphic_map_(main_map),  {

}
void GraphicAgent::Initialize(ServerMap &map, sf::RenderWindow &window) {

}
 */