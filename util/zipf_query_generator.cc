//
// Created by liuzihe on 2020/10/15.
//

#include "util/file.h"
#include "util/coding.h"
#include "util/random.h"

#include "zipf_query_generator.h"

namespace tinykv {
  Status ZipfQueryGenerator::NewZipfQueryGenerator(const std::string& filename,
                                                   int query_times, int first_query_times,
                                                   ZipfQueryGenerator **result) {
    uint64_t file_size;
    Status s = GetFileSize(filename, &file_size);
    CHECK_STATUS(s)

    ZipfQueryGenerator *generator = new ZipfQueryGenerator();
    generator->buf_ = new char[file_size];

    SequentialFile *fin;
    s = NewSequentialFile(filename, &fin);
    CHECK_STATUS(s)

    uint64_t offset;
    uint64_t key_size;
    uint64_t value_size;
    char* ptr = generator->buf_;
    Slice slice;
    float prob = 0;
    for (;;) {
      // Read key_size and key.
      s = fin->Read(8, &slice, ptr);
      BREAK_IFN_OK(s)
      if (slice.Empty()) {
        break;
      }
      offset = ptr - generator->buf_;
      key_size = DecodeFixed64(ptr);
      ptr += 8;
      s = fin->Read(key_size, &slice, ptr);
      BREAK_IFN_OK(s)
      ptr += key_size;

      // Read value_size and value.
      s = fin->Read(8, &slice, ptr);
      BREAK_IFN_OK(s);
      value_size = DecodeFixed64(ptr);
      ptr += 8;
      s = fin->Read(value_size, &slice, ptr);
      BREAK_IFN_OK(s)
      ptr += value_size;

      prob += static_cast<float>(first_query_times) / generator->prob2node_.size() / query_times;
      generator->prob2node_[prob] = Node(offset, key_size, value_size);
    }
    // Safe bound for binary search.
    generator->prob2node_[2] = Node(offset, key_size, value_size);

    *result = generator;
    return Status::OK();
  }

  void ZipfQueryGenerator::Next(Slice* key, Slice* value) {
    float prob = Random::NextFloat();
    auto it = prob2node_.lower_bound(prob);

    char *ptr = buf_ + it->second.offset;
    *key = Slice(ptr + 8, it->second.key_size);
    *value = Slice(ptr + 8 + it->second.key_size + 8, it->second.value_size);
  }
}
