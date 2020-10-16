//
// Created by liuzihe on 2020/10/15.
//

#ifndef TINYKV_FILE_H
#define TINYKV_FILE_H

#include "util/status.h"

namespace tinykv {

  class WritableFile {
  public:
    WritableFile() = default;

    WritableFile(const WritableFile&) = delete;
    WritableFile& operator=(const WritableFile&) = delete;

    virtual ~WritableFile();

    virtual Status Append(const Slice& data) = 0;
    virtual Status Close() = 0;
    virtual Status Flush() = 0;
    virtual Status Sync() = 0;
  };

  inline WritableFile::~WritableFile() = default;
  Status NewWritableFile(const std::string& filename, WritableFile** result);

  class RandomAccessFile {
  public:
    RandomAccessFile() = default;

    RandomAccessFile(const RandomAccessFile&) = delete;
    RandomAccessFile& operator=(const RandomAccessFile&) = delete;

    virtual ~RandomAccessFile();

    virtual Status Read(uint64_t offset, size_t n, Slice* result,
                        char* scratch) const = 0;
  };

  inline RandomAccessFile::~RandomAccessFile() = default;
  Status NewRandomAccessFile(const std::string &filename, RandomAccessFile** result);

  class SequentialFile {
  public:
    SequentialFile() = default;

    SequentialFile(const SequentialFile&) = delete;
    SequentialFile& operator=(const SequentialFile&) = delete;

    virtual ~SequentialFile();

    virtual Status Read(size_t n, Slice* result, char* scratch) = 0;

    virtual Status Skip(uint64_t n) = 0;
  };

  inline SequentialFile::~SequentialFile() = default;
  Status NewSequentialFile(const std::string &filename, SequentialFile **result);

  Status GetFileSize(const std::string& filename, uint64_t* size);

} // End tinykv namespace.

#endif //TINYKV_FILE_H
