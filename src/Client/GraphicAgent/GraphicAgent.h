//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_GAMEGRAPHIC_H_
#define GAME_SRC_GAMEGRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../Angle.h"
#include "../ClientAgent/Map.h"


namespace Client::GraphicAgent {

using Duration = std::chrono::microseconds;

class GraphicAgent {
 public:

  void Initialize(Map &map, sf::RenderWindow &window);

  void GameLoop();

 private:
  Map &map_;
  sf::RenderWindow &window_;

  std::vector<bool> key_pressed_ = std::vector<bool>(sf::Keyboard::KeyCount);

  Duration last_send_info_time_;
  constexpr static auto send_interval = Duration(1000);

  sf::Texture texture_;

 private:
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

}
#endif //GAME_SRC_GAMEGRAPHIC_H_
