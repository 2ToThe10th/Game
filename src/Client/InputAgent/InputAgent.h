//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENTINPUTAGENT_INPUTAGENT_H_
#define GAME_SRC_CLIENTINPUTAGENT_INPUTAGENT_H_

#include <SFML/Window/Event.hpp>
#include <vector>
#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../UserAction.h"


namespace Client::InputAgent {

using Duration = std::chrono::milliseconds;

class InputAgent {
 public:
  InputAgent();

  void Initialize(sf::RenderWindow &window) const;

  void HandleEvent(sf::Event &event);

  [[nodiscard]] UserAction GetUserAction() const;

 private:
  std::vector<bool> key_pressed_ = std::vector<bool>(sf::Keyboard::KeyCount, false);

  Duration last_send_info_time_;
  constexpr static auto kSendInterval = Duration(10);

 private:

  [[nodiscard]] Duration Now() const;

  void HandlerKeyPressed(sf::Event &event);
  void HandlerKeyRelease(sf::Event &event);
  [[nodiscard]] Angle ChooseAngle(Angle angle_left, Angle angle_none, Angle angle_right) const;
  [[nodiscard]] bool IsUpKeyPressed() const;
  [[nodiscard]] bool IsDownKeyPressed() const;
  [[nodiscard]] bool IsRightKeyPressed() const;
  [[nodiscard]] bool IsLeftKeyPressed() const;
};

}

#endif //GAME_SRC_CLIENTINPUTAGENT_INPUTAGENT_H_
