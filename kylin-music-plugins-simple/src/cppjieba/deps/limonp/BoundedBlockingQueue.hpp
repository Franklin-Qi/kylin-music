// Copyright 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LIMONP_BOUNDED_BLOCKING_QUEUE_HPP
#define LIMONP_BOUNDED_BLOCKING_QUEUE_HPP

#include "BoundedQueue.hpp"

namespace limonp {

template<typename T>
class BoundedBlockingQueue : NonCopyable {
 public:
  explicit BoundedBlockingQueue(size_t maxSize)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      queue_(maxSize) {
  }

  void Push(const T& x) {
    MutexLockGuard lock(mutex_);
    while (queue_.Full()) {
      notFull_.Wait();
    }
    assert(!queue_.Full());
    queue_.Push(x);
    notEmpty_.Notify();
  }

  T Pop() {
    MutexLockGuard lock(mutex_);
    while (queue_.Empty()) {
      notEmpty_.Wait();
    }
    assert(!queue_.Empty());
    T res = queue_.Pop();
    notFull_.Notify();
    return res;
  }

  bool Empty() const {
    MutexLockGuard lock(mutex_);
    return queue_.Empty();
  }

  bool Full() const {
    MutexLockGuard lock(mutex_);
    return queue_.Full();
  }

  size_t size() const {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const {
    return queue_.capacity();
  }

 private:
  mutable MutexLock          mutex_;
  Condition                  notEmpty_;
  Condition                  notFull_;
  BoundedQueue<T>  queue_;
}; // class BoundedBlockingQueue

} // namespace limonp

#endif // LIMONP_BOUNDED_BLOCKING_QUEUE_HPP
