//
// Created by liuzihe on 2020/10/16.
//

#include <gtest/gtest.h>

#include "util/cache.h"

namespace tinykv {
  TEST(CacheTest, PutAndGetTest) {
    Cache *cache = NewLRUCache(160);
    for (int i = 0; i < 4; i++) {
      cache->Put(std::to_string(i), std::to_string(i), 1);
    }

    char buf[10];
    Slice result;
    for (int i = 0; i < 4; i++) {
      ASSERT_TRUE(cache->Get(std::to_string(i), &result, buf));
      ASSERT_EQ(result.ToString(), std::to_string(i));
    }

    for (int i = 4; i < 160; i++) {
      cache->Put(std::to_string(i), std::to_string(i), 1);
    }
    ASSERT_FALSE(cache->Get(std::to_string(0), &result, buf));
  }

}

