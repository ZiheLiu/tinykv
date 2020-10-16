//
// Created by liuzihe on 2020/10/16.
//

#ifndef TINYKV_CACHE_H
#define TINYKV_CACHE_H

#include <string>

#include "slice.h"

namespace tinykv {
  class Cache {
  public:
    Cache() = default;
    virtual ~Cache();

    Cache(const Cache&) = delete;
    Cache& operator=(const Cache&) = delete;

    virtual bool Get(const std::string& key, Slice* result, char* payload) = 0;
    virtual void Put(const std::string& key, const std::string& value, size_t charge) = 0;
  };

  inline Cache::~Cache() = default;

  Cache *NewLRUCache(size_t capacity);
}


#endif //TINYKV_CACHE_H
