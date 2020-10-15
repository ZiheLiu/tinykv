//
// Created by liuzihe on 2020/10/15.
//

#include "db/db.h"

namespace tinykv {

  namespace db {
    Status Open(Options& options, DB** result) {
      return options.open_db_func(options.raw_filename, options.index_filename, reinterpret_cast<void **>(result));
    }
  }

}

