//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_OPTIONS_H
#define TINYKV_OPTIONS_H

#include <string>

namespace tinykv {

  struct Options {
    std::string raw_filename;
    std::string index_filename;
    std::string query_filename;

    Status (*open_db_func)(const std::string &raw_filename,
                           const std::string &index_filename,
                           void** result);
  };

}

#endif //TINYKV_OPTIONS_H
