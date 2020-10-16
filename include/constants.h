//
// Created by liuzihe on 2020/10/16.
//

#ifndef TINYKV_CONSTANTS_H
#define TINYKV_CONSTANTS_H

#include <cstdint>

namespace tinykv {
  // 1TB
  constexpr int64_t kGenDataBytes = 1024L * 1024 * 1024 * 1024;
  // ~200W buckets
  constexpr int kHashBucketsBits = 21;
  constexpr int kHashBucketsNum = 1 << kHashBucketsBits;

  constexpr int32_t kKeyBytesMin = 1;
  constexpr int32_t kKeyBytesMax = 1024;
  constexpr int32_t kValueBytesMin = 1;
  constexpr int32_t kValueBytesMax = 1024 * 1024;

  constexpr int32_t kQueryTimes = 100000;
  constexpr int32_t kFirstQueryTimes = 10000;
  constexpr float kSelectQueryProb = 0.01;
  constexpr int32_t kQueryThreadsNum = 8;

  constexpr int32_t kNumCacheShardBits = 4;
  constexpr int32_t kNumCacheShards = 1 << kNumCacheShardBits;
  constexpr int32_t kCacheCapacity = 1024 * 1024 * 2;

#define USE_CACHE
}

#endif //TINYKV_CONSTANTS_H
