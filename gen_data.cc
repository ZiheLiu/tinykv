//
// Created by liuzihe on 2020/10/14.
//

#include <cstdio>

#include "util/file.h"
#include "util/random.h"

namespace tinykv {
  constexpr int64_t kDataBytes = 1024 * 1024 * 1024;

  constexpr int32_t kKeyBytesMin = 1;
  const int32_t kKeyBytesMax = 1024;
  constexpr int32_t kValueBytesMin = 1;
  const int32_t kValueBytesMax = 1024 * 1024;

  Status generate_data() {
    printf("Start generating kv data (%dMB)...\n", kDataBytes / 1024 / 1024);

    WritableFile* fout;
    Status status = NewWritableFile("input_kv.bin", &fout);
    if (!status.ok()) {
      return status;
    }

    random::Init();

    char buf[8 + kKeyBytesMax + 8 + kValueBytesMax];
    int64_t data_bytes = 0;
    while (data_bytes < kDataBytes) {
      char* ptr = buf;
      int64_t key_size = random::RandInt(kKeyBytesMin, kKeyBytesMax);
      memcpy(ptr, &key_size, sizeof(key_size));
      ptr += sizeof(key_size);
      random::RandBytes(ptr, key_size);
      ptr += key_size;

      int64_t value_size = random::RandInt(kValueBytesMin, kValueBytesMax);
      memcpy(ptr, &value_size, sizeof(value_size));
      ptr += sizeof(value_size);
      random::RandBytes(ptr, value_size);
      ptr += value_size;

      status = fout->Append(Slice(buf, ptr - buf));
      if (!status.ok()) {
        return status;
      }

      data_bytes += ptr - buf;
    }

    delete fout;
    return status;
  }
}

int main() {
  tinykv::Status status = tinykv::generate_data();
  if (status.ok()) {
    printf("Generate data successfully.\n");
  } else {
    printf("Generate data failed. %s\n", status.ToString().c_str());
  }

  return 0;
}
