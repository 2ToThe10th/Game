//
// Created by 2ToThe10th on 02.04.2020.
//

#ifndef GAME_SRC_CLIENTGRAPHICAGENT_GRAPHICMAP_H_
#define GAME_SRC_CLIENTGRAPHICAGENT_GRAPHICMAP_H_

#include "../ClientAgent/ClientMap.h"


namespace Client::GraphicAgent {

class GraphicMap {
 public:
  explicit GraphicMap(ClientMap &main_map);

  size_t &LastSynchronize();

  std::vector<std::unique_ptr<Player>> &PlayerVector();

  bool Synchronize();
 private:
 public:
  ClientMap &main_map_;
  std::vector<std::unique_ptr<Player>> players_;
  size_t last_synchronize;
};
}
#endif //GAME_SRC_CLIENTGRAPHICAGENT_GRAPHICMAP_H_
