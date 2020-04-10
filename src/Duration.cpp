//
// Created by 2ToThe10th on 10.04.2020.
//

#include "Duration.h"


namespace Time {

Duration Now() {
  return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now().time_since_epoch());
}

}