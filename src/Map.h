//
// Created by 2ToThe10th on 23.03.2020.
//

#ifndef GAME_SRC_MAP_H_
#define GAME_SRC_MAP_H_

#include <SFML/Graphics/Image.hpp>
#include <vector>
#include <shared_mutex>

#include "Player.h"

class Map {
 public:
  Map() = default;

  virtual void LoadImage(sf::Image &&map_image);

  virtual const sf::Image& GetImage();

  virtual bool ThereIsBarrierBetween(unsigned row, unsigned left_column, unsigned right_column);

  virtual ~Map() = default;
 private:
  std::vector<Player> players_;
  sf::Image map_image_;
  std::vector<uint16_t> prefix_function_barrier_;
  std::shared_mutex mutex_;

 private:
  void GeneratePrefixFunctionBarrier();
};

#endif // GAME_SRC_MAP_H_
