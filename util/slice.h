//
// Created by liuzihe on 2020/10/14.
//

#ifndef TINYKV_SLICE_H
#define TINYKV_SLICE_H

#include <cstddef>
#include <cstring>
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

    bool Empty() const {
      return size_ == 0;
    }

    std::string ToString() const {
      return std::string(data_, size_);
    }

    int compare(const Slice& b) const;

  private:
    const char* data_;
    size_t  size_;
  };

  inline int Slice::compare(const Slice& b) const {
    const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
    int r = memcmp(data_, b.data_, min_len);
    if (r == 0) {
      if (size_ < b.size_)
        r = -1;
      else if (size_ > b.size_)
        r = +1;
    }
    return r;
  }

} // End tinykv namespace.

#endif //TINYKV_SLICE_H
