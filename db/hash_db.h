//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_HASH_DB_H
#define TINYKV_HASH_DB_H

#include "util/status.h"
#include "util/slice.h"
#include "db/db.h"
#include "util/file.h"

namespace tinykv {
  namespace db {
    // ~200W buckets
    constexpr int kHashBucketsNum = 1 << (21 - 6);
    constexpr int kKeyHashSeed = 10247;
    constexpr uint64_t kBucketEmptyMask = -1;

    class HashDB : public DB {
    public:
      HashDB(): fin_(nullptr) {
        memset(buckets_, -1, sizeof(buckets_));
      }
      ~HashDB() override {
        delete fin_;
      }

      static Status OpenDB(const std::string &raw_filename,
                           const std::string &index_filename,
                           void** result);

      Status Get(const Slice &key, Slice* value, char* buf) override;

    private:
      // buckets_[i][0] is offset
      // buckets_[i][1] is key_size
      // buckets_[i][2] is value_size
      uint64_t buckets_[kHashBucketsNum][3];
      RandomAccessFile* fin_;
    };

  }
}

#endif //TINYKV_HASH_DB_H