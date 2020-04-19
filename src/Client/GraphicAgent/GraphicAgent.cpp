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

void GraphicAgent::Initialize(sf::Image &&image, unsigned my_player_id) {
  start_window_size_ = window_.getSize();
  window_.setSize(image.getSize());
  my_player_id_ = my_player_id;
  texture_.loadFromImage(image);
  font_.loadFromFile("../res/ArialMT.ttf");
}

void GraphicAgent::Draw() {
  window_.clear(sf::Color::White);

  window_.draw(sf::Sprite(texture_));

  graphic_map_.Synchronize();

  float scale_x = start_window_size_.x;
  scale_x /= window_.getSize().x;
  float scale_y = start_window_size_.y;
  scale_y /= window_.getSize().y;

  sf::Sprite sprite(texture_);
  sprite.setScale(scale_x, scale_y);
  window_.draw(sprite);

  std::string current_situation;

  int i = 0;
  for (auto& player: graphic_map_.main_map_.players_) {
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
  }

  sf::Text text(current_situation, font_);
  text.setFillColor(sf::Color::Black);

  window_.draw(text);
  current_situation = "";

  i = 0;
  for (auto& player: graphic_map_.main_map_.players_) {
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
  }

  sf::Text text2(current_situation, font_);
  text2.setFillColor(sf::Color::Black);
  text2.setPosition(500, 0);
  window_.draw(text);

  for (int i = 0; i < graphic_map_.PlayerVector().size(); ++i) {
    auto &player = graphic_map_.PlayerVector()[i];
    if (player && i != my_player_id_) {
      DrawPlayer(i, scale_x, scale_y);
    }
  }

  if (graphic_map_.PlayerVector().size() > my_player_id_
      && graphic_map_.PlayerVector()[my_player_id_] != nullptr) {
    DrawPlayer(my_player_id_, scale_x, scale_y, true);
  }

  window_.display();
}

void GraphicAgent::DrawPlayer(unsigned int player_id,
                              float scale_x,
                              float scale_y,
                              bool is_my_player) {
  auto &player = graphic_map_.PlayerVector()[player_id];
  sf::CircleShape circle(kPlayerRadius);
  circle.setPosition(player->GetLocation().GetX() - kPlayerRadius,
                     player->GetLocation().GetY() - kPlayerRadius);
  circle.setScale(scale_x, scale_y);
  if (is_my_player) {
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(kMyPlayerOutlineThickness);
  }
  window_.draw(circle);
}

}


