//
// Created by liuzihe on 2020/10/16.
//

#ifndef TINYKV_CONSTANTS_H
#define TINYKV_CONSTANTS_H

#include <cstdint>

namespace tinykv {
  enum DbPolicy {
    kHashDB = 0
  };

  constexpr int32_t kScaleBits = 3;
  constexpr int32_t kScale = 1 << kScaleBits;

  // For generating data.
  constexpr int64_t kGenDataBytes = 1024L * 1024 * 1024 * 1024 >> kScaleBits;
  constexpr int32_t kKeyBytesMin = 1;
  constexpr int32_t kKeyBytesMax = 1024;
  constexpr int32_t kValueBytesMin = 1;
  constexpr int32_t kValueBytesMax = 1024 * 1024;

  // For generating query data.
  constexpr int32_t kQueryTimes = 100000;
  constexpr int32_t kFirstQueryTimes = 10000;
  constexpr float kSelectQueryProb = 0.01;
  constexpr int32_t kQueryThreadsNum = 8;

  // For HashDB.
  constexpr int kHashBucketsBits = 28 - kScaleBits;
  constexpr int kHashBucketsNum = 1 << kHashBucketsBits;

  // For LRUCache.
#define USE_CACHE
  constexpr int32_t kNumCacheShardBits = 4;
  constexpr int32_t kNumCacheShards = 1 << kNumCacheShardBits;
  constexpr int64_t kCacheCapacity = 1024L * 1024 * 1024 * 2 >> kScaleBits;

  // For db options.
  const std::string kRawFilename = "raw_input_kv.bin";
  const std::string kIndexFilename = "index_input_kv.bin";
  const std::string kQueryFilename = "query_kv.bin";
  constexpr DbPolicy kDbPolicy = kHashDB;
} // End tinykv namespace.

#endif //TINYKV_CONSTANTS_H
