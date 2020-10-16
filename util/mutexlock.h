//
// Created by liuzihe on 2020/10/16.
//

#ifndef TINYKV_MUTEXLOCK_H
#define TINYKV_MUTEXLOCK_H

#include <mutex>

namespace tinykv {

  class MutexLock {
  public:
    explicit MutexLock(std::mutex* mu): mu_(mu) {
        this->mu_->lock();
    }
    ~MutexLock() {
      this->mu_->unlock();
    }

    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;

  private:
    std::mutex* const mu_;
  };

} // End tinykv namespace.

#endif //TINYKV_MUTEXLOCK_H
