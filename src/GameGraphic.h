//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_GAMEGRAPHIC_H_
#define GAME_SRC_GAMEGRAPHIC_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Map.h"
#include "Angel.h"
#include "ClientMap.h"

using Duration = std::chrono::microseconds;

class GameGraphic {
 public:
  explicit GameGraphic(ClientMap &map, sf::Window &window);

  void GameLoop();

 private:
  ClientMap &map_;
  sf::Window &window_;

  bool up_key_pressed_ = false;
  bool left_key_pressed_ = false;
  bool down_key_pressed_ = false;
  bool right_key_pressed_ = false;

  Duration last_send_info_time_;
  constexpr static auto send_interval = Duration(1000);

  sf::Texture texture_;

 private:
  void HandlerEvent(sf::Event& event);
  void HandlerKeyPressed(sf::Event& event);
  void HandlerKeyRelease(sf::Event& event);

  void WatchUserAction();
  [[nodiscard]] Angle ChooseAngle(Angle angle_left, Angle angle_none, Angle angle_right) const;
  void SendUserAction(Angle angle);

  void Draw();

  [[nodiscard]] Duration Now() const;
};

#endif //GAME_SRC_GAMEGRAPHIC_H_
