//
// Created by 2ToThe10th on 02.04.2020.
//

#include "GraphicMap.h"


namespace Client::GraphicAgent {

GraphicMap::GraphicMap(ClientMap &main_map)
    : main_map_(main_map), last_synchronize(std::numeric_limits<size_t>::max()) {
}

bool GraphicMap::Synchronize() {
  return main_map_.Synchronize(*this);
}

size_t &GraphicMap::LastSynchronize() {
  return last_synchronize;
}

std::vector<std::unique_ptr<Player>> &GraphicMap::PlayerVector() {
  return players_;
}

}