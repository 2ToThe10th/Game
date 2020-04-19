//
// Created by 2ToThe10th on 10.04.2020.
//

#ifndef GAME_SRC_DURATION_H_
#define GAME_SRC_DURATION_H_

#include <chrono>

namespace Time {

using Duration = std::chrono::milliseconds;

Duration Now();

}
#endif //GAME_SRC_DURATION_H_
