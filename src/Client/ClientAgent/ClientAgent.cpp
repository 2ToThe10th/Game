//
// Created by 2ToThe10th on 23.03.2020.
//

#include <iostream>
#include "ClientAgent.h"
#include "../../Marshaling.h"


namespace Client {

ClientAgent::ClientAgent(sf::RenderWindow &window)
    : window_(window),
      map_(),
      graphic_agent_(map_, window_),
      input_agent_(),
      tcp_socket_agent_(map_),
      udp_socket_agent_(map_) {
}

void ClientAgent::InitGame(const std::string &host, const size_t tcp_port, const size_t udp_port) {
  input_agent_.Initialize(window_);

  sf::Image image;

  unsigned player_id = tcp_socket_agent_.Initialize(host, tcp_port, image);
  udp_socket_agent_.Initialize(host, udp_port, player_id);

  graphic_agent_.Initialize(std::move(image), player_id);
}

void ClientAgent::RunGame() {
  udp_socket_agent_.WriteToServer(Marshaling::FromInputToUDPMessage(UserAction::Nothing()));
  while (window_.isOpen()) {
    sf::Event event{};
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      } else {
        input_agent_.HandleEvent(event);
      }
    }
    UserAction user_action = input_agent_.GetUserAction();
    if (user_action.HasSomethingToSend()) {
      udp_socket_agent_.WriteToServer(Marshaling::FromInputToUDPMessage(user_action)); // TODO: передавать hash карты и отправлять ключевой кадр только если hash не совпадает
    }
    graphic_agent_.Draw();
  }
}

void ClientAgent::Close() {
  tcp_socket_agent_.Close();
  udp_socket_agent_.Close();
}

} // namespace Client
