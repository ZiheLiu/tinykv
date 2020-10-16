//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_HASH_H
#define TINYKV_HASH_H

#include <cstdint>
#include <cstddef>

namespace tinykv {

  uint32_t Hash(const char* data, size_t n, uint32_t seed);

} // End tinykv namespace.

#endif //TINYKV_HASH_H
