//
// Created by 2ToThe10th on 23.03.2020.
//

#include <iostream>
#include <cmath>
#include "GraphicAgent.h"
#include "../../Angle.h"


namespace Client::GraphicAgent {

const sf::Color GraphicAgent::kMyPlayerOutlineColor = sf::Color::Red;

GraphicAgent::GraphicAgent(ClientMap &main_map, sf::RenderWindow &window)
    : graphic_map_(main_map), window_(window) {

}

void GraphicAgent::Initialize(sf::Image &&image, unsigned my_player_id) {
  start_window_size_ = window_.getSize();
  my_player_id_ = my_player_id;
  texture_.loadFromImage(image);
  font_.loadFromFile("../res/ArialMT.ttf");
}

void GraphicAgent::Draw() {
  graphic_map_.Synchronize();

  if (graphic_map_.PlayerVector().size() <= my_player_id_
      || graphic_map_.PlayerVector()[my_player_id_] == nullptr) {
    window_.clear(sf::Color::White);
    sf::Text text("Connecting", font_);
    text.setFillColor(sf::Color::Black);
    window_.draw(text);
    window_.display();
    return;
  }

  window_.clear(sf::Color::White);

  UpdateScale();
  UpdateStartWindowLocation();

  DrawBackGround();

  DrawGrid();

  for (int i = 0; i < graphic_map_.PlayerVector().size(); ++i) {
    auto &player = graphic_map_.PlayerVector()[i];
    if (player && i != my_player_id_) {
      DrawPlayer(i, false);
    }
  }

  DrawPlayer(my_player_id_, true);

  window_.display();
}

void GraphicAgent::DrawPlayer(unsigned int player_id, bool is_my_player) {
  auto &player = graphic_map_.PlayerVector()[player_id];
  sf::CircleShape circle(kPlayerRadius, 1000);
  circle.setPosition(ScaledPosition(
      player->GetLocation().GetX() - kPlayerRadius - start_window_location_.GetX(),
      player->GetLocation().GetY() - kPlayerRadius - start_window_location_.GetY()));
  circle.setScale(scale_x_, scale_y_);
  if (is_my_player) {
    circle.setOutlineColor(kMyPlayerOutlineColor);
    circle.setOutlineThickness(kMyPlayerOutlineThickness);
  }
  window_.draw(circle);
}

void GraphicAgent::UpdateScale() {
  scale_x_ = start_window_size_.x;
  scale_x_ /= window_.getSize().x;
  scale_y_ = start_window_size_.y;
  scale_y_ /= window_.getSize().y;
}

sf::Vector2f GraphicAgent::ScaledPosition(float position_x, float position_y) const {
  return sf::Vector2f(position_x * scale_x_, position_y * scale_y_);
}

sf::Vector2f GraphicAgent::ScaledPosition(sf::Vector2f position) const {
  return ScaledPosition(position.x, position.y);
}

void GraphicAgent::DrawGrid() {
  for (int i = 0; i < window_.getSize().y; i += 100) {
    sf::RectangleShape line(sf::Vector2f(window_.getSize().x * scale_x_, scale_y_));

    line.setPosition(ScaledPosition(0, static_cast<float>(i)));

    line.setFillColor(sf::Color::Green);

    window_.draw(line);
  }

  for (int i = 0; i < window_.getSize().x; i += 100) {
    sf::RectangleShape line(sf::Vector2f(scale_x_, window_.getSize().y * scale_y_));

    line.setPosition(ScaledPosition(static_cast<float>(i), 0));

    line.setFillColor(sf::Color::Blue);

    window_.draw(line);
  }
}

void GraphicAgent::DrawBackGround() {
  sf::Sprite sprite(texture_,
                    sf::IntRect(start_window_location_.GetX(),
                                start_window_location_.GetY(),
                                std::min(GetXLeftToDraw(), window_.getSize().x),
                                std::min(GetYLeftToDraw(), window_.getSize().y)));
  sprite.setScale(scale_x_, scale_y_);
  window_.draw(sprite);
}

void GraphicAgent::UpdateStartWindowLocation() {
  Location my_player_location = graphic_map_.PlayerVector()[my_player_id_]->GetLocation();
  start_window_location_ =
      Location(my_player_location.GetX() - static_cast<float>(window_.getSize().x) / 2,
               my_player_location.GetY() - static_cast<float>(window_.getSize().y) / 2);
  if (start_window_location_.GetX() + window_.getSize().x > texture_.getSize().x) {
    start_window_location_.SetX(static_cast<float>(texture_.getSize().x) - window_.getSize().x);
  }
  if (start_window_location_.GetY() + window_.getSize().y > texture_.getSize().y) {
    start_window_location_.SetY(static_cast<float>(texture_.getSize().y) - window_.getSize().y);
  }
  if (start_window_location_.GetX() < 0) {
    start_window_location_.SetX(0);
  }
  if (start_window_location_.GetY() < 0) {
    start_window_location_.SetY(0);
  }
}

unsigned GraphicAgent::GetXLeftToDraw() {
  return texture_.getSize().x - static_cast<unsigned>(std::lround(start_window_location_.GetX()));
}

unsigned GraphicAgent::GetYLeftToDraw() {
  return texture_.getSize().y - static_cast<unsigned>(std::lround(start_window_location_.GetY()));
}

}


