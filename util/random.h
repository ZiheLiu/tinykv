//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_RANDOM_H
#define TINYKV_RANDOM_H

#include <ctime>
#include <cstdlib>

namespace tinykv {

  class Random {
  public:
    static void Init() {
      srand(static_cast<unsigned int>(time(NULL)));
    }

    // Generate random integer in [left, right).
    static int NextInt(int left, int right) {
      return rand() % (right - left) + left;
    }

    static char NextByte() {
      return rand() % 256;
    }

    static void NextBytes(char* result, int n) {
      for (int i = 0; i < n; i++) {
        result[i] = rand() % 256;
      }
    }

    static float NextFloat() {
      return static_cast<float>(rand()) / RAND_MAX;
    }
  };

} // End tinykv namespace.

#endif //TINYKV_RANDOM_H
