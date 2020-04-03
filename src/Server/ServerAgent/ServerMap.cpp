//
// Created by
// 2ToThe10th
// on 23.03.2020.
//

#include "ServerMap.h"

#include <iostream>
#include <mutex>


namespace Server {

void ServerMap::LoadImageFromFile(const std::string &image_file) {
  if (!background_.loadFromFile(image_file)) {
    throw std::system_error();
  }
  std::cout << "Ok" << std::endl;
}

const sf::Image &ServerMap::GetImage() {
  return background_;
}
}  // namespace Server
