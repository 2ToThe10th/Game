//
// Created by 2ToThe10th on 23.03.2020.
//

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

  tcp_socket_agent_.Initialize(host, tcp_port, image);
  udp_socket_agent_.Initialize(host, udp_port);

  graphic_agent_.Initialize(std::move(image));
}

void ClientAgent::RunGame() {
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
      udp_socket_agent_.WriteToServer(Marshaling::FromInputToUDPSocket(user_action));
    }
    graphic_agent_.Draw();
  }
}

void ClientAgent::Close() {
  tcp_socket_agent_.Close();
  udp_socket_agent_.Close();
}

} // namespace Client

/*
void ClientAgent::InitGame(const std::string &host,
                           size_t udp_port,
                           size_t tcp_port,
                           sf::RenderWindow &window) {
  ServerMap map;

  sf::Image image;
  if (!image.loadFromFile("/home/sasha/Game/res/ServerMap.png")) {
    std::cout << "oy" << std::endl;
    return;
  }
//  GraphicAgent::GraphicAgent game_graphic(map, window);

//  game_graphic.GameLoop();
}
*/