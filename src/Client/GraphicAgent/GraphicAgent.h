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

};
}
#endif //GAME_SRC_GAMEGRAPHIC_H_


/*



 private:
  void Initialize(GraphicMap &map, sf::RenderWindow &window);

  void GameLoop();

  void HandlerEvent(sf::Event &event);

  void HandlerKeyPressed(sf::Event &event);

  void HandlerKeyRelease(sf::Event &event);

  [[nodiscard]] inline bool IsUpKeyPressed() const;

  [[nodiscard]] inline bool IsDownKeyPressed() const;

  [[nodiscard]] inline bool IsRightKeyPressed() const;

  [[nodiscard]] inline bool IsLeftKeyPressed() const;

  void WatchUserAction();

  [[nodiscard]] Angle ChooseAngle(Angle angle_left, Angle angle_none, Angle angle_right) const;

  void SendUserAction(Angle angle);

  void Draw();

  [[nodiscard]] Duration Now() const;
};

*/