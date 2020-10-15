//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_ZIPF_QUERY_GENERATOR_H
#define TINYKV_ZIPF_QUERY_GENERATOR_H


#include <cstdint>
#include <map>

#include "util/slice.h"

namespace tinykv {

  class ZipfQueryGenerator {
  public:
    ZipfQueryGenerator() = default;
    ~ZipfQueryGenerator() {
      delete buf_;
    }

    ZipfQueryGenerator(const ZipfQueryGenerator& other) = delete;
    ZipfQueryGenerator& operator=(const ZipfQueryGenerator& other) = delete;

    static Status NewZipfQueryGenerator(const std::string& filename,
                                        int query_times, int first_query_times,
                                        ZipfQueryGenerator** result);

    void Next(Slice *key, Slice *value);

  private:
    struct Node {
      uint64_t offset;
      uint64_t key_size;
      uint64_t value_size;

      Node() {}

      Node(uint64_t offset, uint64_t key_size, uint64_t value_size):
        offset(offset), key_size(key_size), value_size(value_size) {}
    };

    char* buf_;
    std::map<float, Node> prob2node_;
  };

}

#endif //TINYKV_ZIPF_QUERY_GENERATOR_H
