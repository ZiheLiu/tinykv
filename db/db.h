//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_DB_H
#define TINYKV_DB_H

#include "util/slice.h"
#include "util/status.h"
#include "db/options.h"

namespace tinykv {

namespace db {
  class DB {
  public:
    DB() = default;
    virtual ~DB();

    virtual Status Get(const Slice &key, Slice* value, char* buf) = 0;

    DB(const DB& other) = delete;
    DB& operator=(const DB& other) = delete;
  };

  Status Open(Options& options, DB** result);

  inline DB::~DB() = default;
}

}

#endif //TINYKV_DB_H
