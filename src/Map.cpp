//
// Created by 2ToThe10th on 23.03.2020.
//

#include "Map.h"

#include <mutex>

// sf::Image don't have copy and constructor from rvalue,
// so compiler will move array, and copy size (O(1))
// based on cppreference
void Map::LoadImage(sf::Image &&map_image) {
  std::unique_lock lock(mutex_);
  map_image_ = map_image;
  GeneratePrefixFunctionBarrier();
}

void Map::GeneratePrefixFunctionBarrier() {
  auto image_size = map_image_.getSize();
  prefix_function_barrier_.resize(image_size.x * (image_size.y + 1));

  for (unsigned row = 0; row < image_size.x; ++row) {
    unsigned element_before_number = row * (image_size.y + 1);
    prefix_function_barrier_[element_before_number] = 0;

    for (unsigned column = 0; column < image_size.y; ++column) {

      prefix_function_barrier_[element_before_number + column + 1] =
          prefix_function_barrier_[element_before_number + column]
              + (map_image_.getPixel(row, column) == sf::Color::Black ? 1 : 0);

    }
  }
}

bool Map::ThereIsBarrierBetween(unsigned row,
                                unsigned left_column,
                                unsigned right_column) {
  std::shared_lock lock(mutex_);
  unsigned element_before_number = row * (map_image_.getSize().y + 1);

  return prefix_function_barrier_[element_before_number + left_column]
      != prefix_function_barrier_[element_before_number + right_column + 1];
}

const sf::Image &Map::GetImage() {
  return map_image_;
}
