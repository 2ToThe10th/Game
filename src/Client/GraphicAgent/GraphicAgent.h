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

  void Initialize(sf::Image &&image);

  void Draw();

 private:

  GraphicMap graphic_map_;
  sf::RenderWindow &window_;
  sf::Texture texture_;

  sf::Font font_; // TODO: delete
};
}
#endif //GAME_SRC_GAMEGRAPHIC_H_
