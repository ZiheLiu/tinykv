//
// Created by liuzihe on 2020/10/15.
//

#include "db/db.h"
#include "db/hash_db.h"
#include "constants.h"

namespace tinykv {

  namespace db {
    Status Open(const std::string& raw_filename, const std::string& index_filename, DbPolicy policy, DB** result) {
      switch (policy) {
        case kHashDB:
          return HashDB::OpenDB(raw_filename, index_filename, result);
        default:
          return Status::InvalidArgument("DbPolicy [" + std::to_string(policy) + "]");
      }
    }
  }

}

