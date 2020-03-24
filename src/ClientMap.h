//
// Created by 2ToThe10th on 24.03.2020.
//

#ifndef GAME_SRC_CLIENTMAP_H_
#define GAME_SRC_CLIENTMAP_H_

#include "Map.h"

class ClientMap: public Map {
 public:
  ClientMap() = default;

  void LoadImage(sf::Image &&map_image) override {
    Map::LoadImage(std::move(map_image));
    is_changed = true;
  }

  bool IsChanged() {
    return is_changed;
  }

  ~ClientMap() override = default;
 private:
  bool is_changed = false;
};

#endif //GAME_SRC_CLIENTMAP_H_
