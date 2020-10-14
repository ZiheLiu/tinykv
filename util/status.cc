//
// Created by liuzihe on 2020/10/14.
//

#include "status.h"

namespace tinykv {

const char * Status::CopyState(const char *s) {
  uint32_t len;
  memcpy(&len, s, sizeof(len));
  char *result = new char[len + 5];
  memcpy(result, s, len + 5);
  return result;
}

std::string Status::ToString() {
  if (state_ == nullptr) {
    return "OK";
  }

  std::string result;
  switch (code()) {
    case kOk:
      result.append("OK");
      break;
    case kIOError:
      result.append("IO error: ");
      break;
    default:
      result.append("Unknown code(");
      result.append(std::to_string(static_cast<int>(result[4])));
      result.append("): ");
  }

  uint32_t len;
  memcpy(&len, state_, sizeof(len));
  result.append(state_ + 5, len);

  return result;
}

Status::Status(Code code, const Slice &msg) {
  assert(code != kOk);
  uint32_t len = msg.size();
  char* buffer = new char[len + 5];
  memcpy(buffer, &len, sizeof(len));
  buffer[4] = static_cast<char>(code);
  memcpy(buffer + 5, msg.data(), len);

  state_ = buffer;
}

}

