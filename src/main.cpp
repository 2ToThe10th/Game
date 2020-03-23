//
// Created by 2ToThe10th on 23.03.2020.
//
#ifdef DEBUG
#include <gtest/gtest.h>
#endif

int main() {

#ifdef DEBUG
  ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

  return 0;
}