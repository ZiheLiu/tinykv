//
// Created by liuzihe on 2020/10/14.
//

#ifndef tinykv_STATUS_H
#define tinykv_STATUS_H

#include "slice.h"

namespace tinykv {

class Status {
public:
  Status(): state_(nullptr) {}
  ~Status() {
    delete[] state_;
  }

  Status(const Status& other);
  Status& operator=(const Status& other);

  static Status OK() {
    return {};
  }

  static Status IOError(const Slice& msg) {
    return {kIOError, msg};
  }

  bool ok() const {
    return state_ == nullptr;
  }

  bool IsIOError() const {
    return code() == kIOError;
  }

  std::string ToString();

private:
  enum Code {
    kOk = 0,
    kIOError = 1
  };

  // OK status has a null state_.
  // Otherwise, state_ is a array with the following three items:
  // state_[4] == code
  // state_[0..3] == length of message
  // state_[5..] == message
  const char* state_;

  Status(Code code, const Slice& msg);

  Code code() const {
    return state_ == nullptr ? kOk : static_cast<Code>(state_[4]);
  }

  static const char* CopyState(const char* s);
};

inline Status::Status(const Status &other) {
  state_ = (other.state_ == nullptr) ? nullptr : CopyState(other.state_);
}

inline Status & Status::operator=(const Status &other) {
  if (other.state_ != state_) {
    delete[] state_;
    state_ = (other.state_ == nullptr) ? nullptr : CopyState(other.state_);
  }
  return *this;
}

}

#endif //tinykv_STATUS_H
