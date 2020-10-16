//
// Created by liuzihe on 2020/10/16.
//

#include <gtest/gtest.h>

#include "util/coding.h"

namespace tinykv {

  TEST(CodingTest, Varint32Test) {
    uint64_t nums[] = {1000, 10000, 1024 * 1024};
    char buf[5];
    uint64_t result;

    for (uint64_t num : nums) {
      char *ptr = EncodeVarint64(buf, num);
      char *ptr2 = DecodeVarint64(buf, &result);
      ASSERT_EQ(result, num);
      ASSERT_EQ(ptr, ptr2);
    }
  }

}

