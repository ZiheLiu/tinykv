//
// Created by liuzihe on 2020/10/15.
//

#include "db/db.h"
#include "util/file.h"
#include "util/hash.h"
#include "db/hash_db.h"
#include "util/coding.h"

namespace tinykv {
  namespace db {

    static uint32_t KeyHash(const char* key, int64_t key_size) {
      return Hash(key, key_size, kKeyHashSeed) & (kHashBucketsNum - 1);
    }

    Status HashDB::OpenDB(const std::string &raw_filename,
                          const std::string &index_filename,
                          void** result) {
      SequentialFile *fin;
      Status s = NewSequentialFile(raw_filename, &fin);
      CHECK_STATUS(s)
      WritableFile *fout;
      s = NewWritableFile(index_filename, &fout);
      if (!s.ok()) {
        delete fin;
        return s;
      }

      HashDB *db = new HashDB();
      *result = nullptr;

      uint64_t out_offset = 0;
      uint64_t key_size;
      uint64_t value_size;
      char size_buf[sizeof(key_size)];
      char buf[1024 + 1024 * 1024 + 8 * 3];
      Slice slice;
      for (;;) {
        // Read key_size.
        s = fin->Read(8, &slice, size_buf);
        if (!s.ok()) {
          break;
        }
        if (slice.Empty()) {
          break;
        }
        key_size = DecodeFixed64(size_buf);
        // Read key.
        s = fin->Read(key_size, &slice, buf);
        if (!s.ok()) {
          break;
        }

        // Read value_size.
        s = fin->Read(8, &slice, size_buf);
        if (!s.ok()) {
          break;
        }
        value_size = DecodeFixed64(size_buf);
        // Read value.
        s = fin->Read(value_size, &slice, buf + key_size);

        uint32_t h = KeyHash(buf, key_size);
        uint64_t *bucket = db->buckets_[h];

        // Write to index file.
        // key|value|pre_offset_in_same_bucket|pre_key_size|pre_value_size.
        EncodeFixed64(buf + key_size + value_size, bucket[0]);
        EncodeFixed64(buf + key_size + value_size + 8, bucket[1]);
        EncodeFixed64(buf + key_size + value_size + 16, bucket[2]);
        s = fout->Append(Slice(buf, key_size + value_size + 8 * 3));
        if (!s.ok()) {
          break;
        }

        bucket[0] = out_offset;
        bucket[1] = key_size;
        bucket[2] = value_size;

        out_offset += 16 + key_size + value_size + 8;
      }

      delete fin;
      delete fout;
      if (!s.ok()) {
        delete db;
        return s;
      }

      s = NewRandomAccessFile(index_filename, &db->fin_);
      if (!s.ok()) {
        delete db;
        return s;
      }

      *result = db;
      return s;
    }

    Status HashDB::Get(const Slice &key, Slice* value, char* buf) {
      uint32_t h = KeyHash(key.data(), key.size());
      uint64_t *bucket = buckets_[h];
      uint64_t offset = bucket[0];
      uint64_t key_size = bucket[1];
      uint64_t value_size = bucket[2];

      Slice result;
      for (;;) {
        // This bucket has no more node.
        if (offset == kBucketEmptyMask) {
          return Status::NotFound(key);
        }

        Status status = fin_->Read(offset, key_size + value_size + 8 * 3, &result, buf);
        if (!status.ok()) {
          return status;
        }

        // Find this key.
        if (key.compare(Slice(buf, key_size)) == 0) {
          *value = Slice(buf + key_size, value_size);
          return Status::OK();
        }

        offset = DecodeFixed64(buf + key_size + value_size);
        key_size = DecodeFixed64(buf + key_size + value_size + 8);
        value_size = DecodeFixed64(buf + key_size + value_size + 16);
      }
    }

  }
}

