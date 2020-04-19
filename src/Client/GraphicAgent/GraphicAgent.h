//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_GAMEGRAPHIC_H_
#define GAME_SRC_GAMEGRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../Angle.h"
#include "../ClientAgent/ClientMap.h"
#include "GraphicMap.h"


namespace Client::GraphicAgent {

class GraphicAgent {
 public:
  GraphicAgent(ClientMap &main_map, sf::RenderWindow &window);

  void Initialize(sf::Image &&image, unsigned my_player_id);

  void Draw();

 private:
  void DrawPlayer(unsigned int player_id, float scale_x, float scale_y, bool is_my_player = false);

 private:
  static constexpr float kPlayerRadius = 5;
  static constexpr float kMyPlayerOutlineThickness = 3;

  sf::Vector2u start_window_size_;
  unsigned my_player_id_;
  GraphicMap graphic_map_;
  sf::RenderWindow &window_;
  sf::Texture texture_;

  sf::Font font_; // TODO: delete
};
}
#endif //GAME_SRC_GAMEGRAPHIC_H_
