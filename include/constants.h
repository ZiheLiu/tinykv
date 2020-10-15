//
// Created by liuzihe on 2020/10/16.
//

#ifndef TINYKV_CONSTANTS_H
#define TINYKV_CONSTANTS_H

#include <cstdint>

namespace tinykv {
  // 1TB
  constexpr int64_t kGenDataBytes = 1024L * 1024 * 1024 * 16;
  // ~200W buckets
  constexpr int kHashBucketsNum = 1 << (21 - 6);

  constexpr int32_t kKeyBytesMin = 1;
  constexpr int32_t kKeyBytesMax = 1024;
  constexpr int32_t kValueBytesMin = 1;
  constexpr int32_t kValueBytesMax = 1024 * 1024;

  constexpr int32_t kQueryTimes = 10000;
  constexpr int32_t kFirstQueryTimes = 1000;
  constexpr int32_t kQueryThreadsNum = 8;
}

#endif //TINYKV_CONSTANTS_H
