//
// Created by liuzihe on 2020/10/14.
//

#include <cstdio>
#include <algorithm>

#include "util/file.h"
#include "util/random.h"
#include "util/coding.h"
#include "constants.h"

namespace tinykv {

  Status generate_data() {
    printf("Start generating kv data (%ldMB in total)...\n", kGenDataBytes / 1024 / 1024);

    WritableFile* fout;
    Status status = NewWritableFile("raw_input_kv.bin", &fout);
    RETURN_IFN_OK(status)

    WritableFile* query_fout;
    status = NewWritableFile("query_kv.bin", &query_fout);
    RETURN_IFN_OK(status)

    Random::Init();

    char buf[8 + kKeyBytesMax + 8 + kValueBytesMax];
    uint64_t data_bytes = 0;
    int kv_num = 0;
    int query_times = 0;
    int query_kv_num = 0;
    while (data_bytes < kGenDataBytes) {
      char* ptr = buf;
      uint64_t key_size = Random::NextInt(kKeyBytesMin, kKeyBytesMax);
      EncodeFixed64(ptr, key_size);
      ptr += sizeof(key_size);
      Random::NextBytes(ptr, key_size);
      ptr += key_size;

      uint64_t value_size = Random::NextInt(kValueBytesMin, kValueBytesMax);
      EncodeFixed64(ptr, value_size);
      ptr += sizeof(value_size);
      Random::NextBytes(ptr, value_size);
      ptr += value_size;

      status = fout->Append(Slice(buf, ptr - buf));
      RETURN_IFN_OK(status)

      data_bytes += ptr - buf;
      kv_num++;

      if (query_times < kQueryTimes && Random::NextFloat() < 0.2) {
        int cur_query_times = kFirstQueryTimes / (++query_kv_num);
        query_times += cur_query_times > 1 ? cur_query_times : 1;
        status = query_fout->Append(Slice(buf, ptr - buf));
        RETURN_IFN_OK(status)
      }
    }

    printf("%d kv pairs in total.\n", kv_num);
    printf("%d query kv pairs in total.\n", query_kv_num);

    delete fout;
    delete query_fout;
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
