//
// Created by liuzihe on 2020/10/14.
//

#ifndef tinykv_SLICE_H
#define tinykv_SLICE_H

#include <cstddef>
#include <string>

namespace tinykv {

class Slice {
public:
  Slice(): data_(""), size_(0) {}

  Slice(const std::string& s): data_(s.data()), size_(s.size()) {}

  Slice(const char* data, size_t size): data_(data), size_(size) {}

  Slice(const char* s): data_(s), size_(strlen(s)) {}

  // Intentionally copyable.
  Slice(const Slice&) = default;
  Slice& operator=(const Slice&) = default;

  size_t size() const {
    return size_;
  }

  const char* data() const {
    return data_;
  }

  bool Empty() {
    return size_ == 0;
  }

  std::string ToString() {
    return std::string(data_, size_);
  }

private:
  const char* data_;
  size_t  size_;
};

}

#endif //tinykv_SLICE_H
