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
 private:
 public: //TODO: delete it
  ClientMap &main_map_;
  size_t last_syncronize;
};
}
#endif //GAME_SRC_CLIENTGRAPHICAGENT_GRAPHICMAP_H_
