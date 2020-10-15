//
// Created by liuzihe on 2020/10/15.
//

#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include "file.h"

namespace tinykv {

namespace {
  constexpr const size_t kWritableFileBufferSize = 65536;

  Status PosixError(const std::string& context, int error_number) {
    std::string msg;
    msg.append(context);
    msg.append(" ");
    msg.append(std::strerror(error_number));
    return Status::IOError(msg);
  }

  class PosixWritableFile : public WritableFile {
  public:
    PosixWritableFile(std::string filename, int fd):
      fd_(fd), pos_(0), filename_(std::move(filename)) {}

    ~PosixWritableFile() override {
      if (fd_ > 0) {
        Close();
      }
    }

    Status Append(const Slice &data) override {
      size_t write_size = data.size();
      const char* write_data = data.data();

      // Fit as much as possible into buffer.
      size_t copy_size = std::min(write_size, kWritableFileBufferSize - pos_);
      std::memcpy(buf_ + pos_, write_data, copy_size);
      write_data += copy_size;
      write_size -= copy_size;
      pos_ += copy_size;
      if (write_size == 0) {
        return Status::OK();
      }

      // Can't fit in buffer, so need to do at least one write.
      Status status = FlushBuffer();
      if (!status.ok()) {
        return status;
      }

      // Small writes go to buffer, large writes are written directly.
      if (write_size < kWritableFileBufferSize) {
        std::memcpy(buf_, write_data, write_size);
        pos_ = write_size;
        return Status::OK();
      }
      return WriteUnbuffered(write_data, write_size);
    }

    Status Close() override {
      Status status = FlushBuffer();
      const int close_result = ::close(fd_);
      if (close_result < 0 && status.ok()) {
        status = PosixError(filename_, errno);
      }
      fd_ = -1;
      return status;
    }

    Status Flush() override {
      return FlushBuffer();
    }

    Status Sync() override {
      bool sync_success = ::fsync(fd_) == 0;
      if (sync_success) {
        return Status::OK();
      }
      return PosixError(filename_, errno);
    }

  private:
    char buf_[kWritableFileBufferSize];
    size_t pos_;
    int fd_;

    const std::string filename_;

    Status FlushBuffer() {
      Status status = WriteUnbuffered(buf_, pos_);
      pos_ = 0;
      return status;
    }

    Status WriteUnbuffered(const char* data, size_t size) {
      while (size > 0) {
        ssize_t write_result = ::write(fd_, data, size);
        if (write_result < 0) {
          if (errno == EINTR) {
            continue;  // Retry
          }
          return PosixError(filename_, errno);
        }
        data += write_result;
        size -= write_result;
      }
      return Status::OK();
    }
  };

  class PosixRandomAccessFile : public RandomAccessFile {
  public:
    PosixRandomAccessFile(std::string filename, int fd):
      fd_(fd), filename_(std::move(filename)) {}

    ~PosixRandomAccessFile() override {
      ::close(fd_);
    }

    Status Read(uint64_t offset, size_t n, Slice *result, char *scratch) const override {
      Status status;
      ssize_t read_size = ::pread(fd_, scratch, n, static_cast<off_t>(offset));
      *result = Slice(scratch, (read_size < 0) ? 0 : read_size);
      if (read_size < 0) {
        // An error: return a non-ok status.
        status = PosixError(filename_, errno);
      }
      return status;
    }

  private:
    int fd_;
    const std::string filename_;
  };
}

Status NewWritableFile(const std::string& filename, WritableFile** result) {
  int fd = ::open(filename.c_str(), O_TRUNC | O_WRONLY | O_CREAT, 0644);
  if (fd < 0) {
    *result = nullptr;
    return PosixError(filename, errno);
  }

  *result = new PosixWritableFile(filename, fd);
  return Status::OK();
}

Status NewRandomAccessFile(const std::string& filename, RandomAccessFile** result) {
  int fd = ::open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
    *result = nullptr;
    return PosixError(filename, errno);
  }

  *result = new PosixRandomAccessFile(filename, fd);
  return Status::OK();
}



}