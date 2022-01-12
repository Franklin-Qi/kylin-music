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

#ifndef LIMONP_BOUNDED_QUEUE_HPP
#define LIMONP_BOUNDED_QUEUE_HPP

#include <vector>
#include <fstream>
#include <cassert>

namespace limonp {
using namespace std;
template<class T>
class BoundedQueue {
 public:
  explicit BoundedQueue(size_t capacity): capacity_(capacity), circular_buffer_(capacity) {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
    assert(capacity_);
  }
  ~BoundedQueue() {
  }

  void Clear() {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
  }
  bool Empty() const {
    return !size_;
  }
  bool Full() const {
    return capacity_ == size_;
  }
  size_t Size() const {
    return size_;
  }
  size_t Capacity() const {
    return capacity_;
  }

  void Push(const T& t) {
    assert(!Full());
    circular_buffer_[tail_] = t;
    tail_ = (tail_ + 1) % capacity_;
    size_ ++;
  }

  T Pop() {
    assert(!Empty());
    size_t oldPos = head_;
    head_ = (head_ + 1) % capacity_;
    size_ --;
    return circular_buffer_[oldPos];
  }

 private:
  size_t head_;
  size_t tail_;
  size_t size_;
  const size_t capacity_;
  vector<T> circular_buffer_;

}; // class BoundedQueue
} // namespace limonp

#endif
