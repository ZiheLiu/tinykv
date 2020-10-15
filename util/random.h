//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_RANDOM_H
#define TINYKV_RANDOM_H

#include <ctime>
#include <cstdlib>

namespace tinykv {

namespace random {
  void Init() {
    srand(static_cast<unsigned int>(time(NULL)));
  }

  // Generate random integer in [left, right).
  int RandInt(int left, int right) {
    return rand() % (right - left) + left;
  }

  char RandByte() {
    return rand() % 256;
  }

  void RandBytes(char* result, int n) {
    for (int i = 0; i < n; i++) {
      result[i] = rand() % 256;
    }
  }
}

}

#endif //TINYKV_RANDOM_H
