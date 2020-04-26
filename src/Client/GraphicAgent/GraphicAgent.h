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
  void DrawPlayer(unsigned int player_id, bool is_my_player = false);
  void DrawGrid();
  void DrawBackGround();

  void UpdateScale();
  void UpdateStartWindowLocation();

  unsigned GetXLeftToDraw();
  unsigned GetYLeftToDraw();

  inline sf::Vector2f ScaledPosition(sf::Vector2f position) const;
  inline sf::Vector2f ScaledPosition(float position_x, float position_y) const;

 private:
  static constexpr float kPlayerRadius = 5;
  static constexpr float kMyPlayerOutlineThickness = 2;
  static const sf::Color kMyPlayerOutlineColor;

  sf::Vector2u start_window_size_;
  float scale_x_ = 1;
  float scale_y_ = 1;
  unsigned my_player_id_;
  Location start_window_location_;
  GraphicMap graphic_map_;
  sf::RenderWindow &window_;
  sf::Texture texture_;

  sf::Font font_; // TODO: delete
};
}
#endif //GAME_SRC_GAMEGRAPHIC_H_
