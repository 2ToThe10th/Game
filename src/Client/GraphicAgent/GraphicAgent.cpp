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


