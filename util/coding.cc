//
// Created by liuzihe on 2020/10/16.
//

#include "coding.h"

namespace tinykv {

  char* DecodeVarint64(const char* ptr, uint64_t* dst) {
    uint64_t num = 0;
    uint64_t shift = 0;
    for (;;) {
      uint64_t cur = static_cast<uint64_t>((*ptr) & 0xff);
      num |= (cur & 0x7f) << (shift * 7);
      ptr++;
      if (!(cur & 128)) {
        break;
      }
      shift++;
    }
    *dst = num;
    return const_cast<char *>(ptr);
  }

  char* EncodeVarint64(char *dst, uint64_t value) {
    static const int B = 128;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
    while (value >= B) {
      *(ptr++) = value | B;
      value >>= 7;
    }
    *(ptr++) = static_cast<uint8_t>(value);
    return reinterpret_cast<char*>(ptr);
  }

} // End tinykv namespace.