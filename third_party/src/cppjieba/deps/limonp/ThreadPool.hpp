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

#ifndef LIMONP_THREAD_POOL_HPP
#define LIMONP_THREAD_POOL_HPP

#include "Thread.hpp"
#include "BlockingQueue.hpp"
#include "BoundedBlockingQueue.hpp"
#include "Closure.hpp"

namespace limonp {

using namespace std;

//class ThreadPool;
class ThreadPool: NonCopyable {
 public:
  class Worker: public IThread {
   public:
    Worker(ThreadPool* pool): ptThreadPool_(pool) {
      assert(ptThreadPool_);
    }
    virtual ~Worker() {
    }

    virtual void Run() {
      while (true) {
        ClosureInterface* closure = ptThreadPool_->queue_.Pop();
        if (closure == NULL) {
          break;
        }
        try {
          closure->Run();
        } catch(std::exception& e) {
          XLOG(ERROR) << e.what();
        } catch(...) {
          XLOG(ERROR) << " unknown exception.";
        }
        delete closure;
      }
    }
   private:
    ThreadPool * ptThreadPool_;
  }; // class Worker

  ThreadPool(size_t thread_num)
    : threads_(thread_num), 
      queue_(thread_num) {
    assert(thread_num);
    for(size_t i = 0; i < threads_.size(); i ++) {
      threads_[i] = new Worker(this);
    }
  }
  ~ThreadPool() {
    Stop();
  }

  void Start() {
    for(size_t i = 0; i < threads_.size(); i++) {
      threads_[i]->Start();
    }
  }
  void Stop() {
    for(size_t i = 0; i < threads_.size(); i ++) {
      queue_.Push(NULL);
    }
    for(size_t i = 0; i < threads_.size(); i ++) {
      threads_[i]->Join();
      delete threads_[i];
    }
    threads_.clear();
  }

  void Add(ClosureInterface* task) {
    assert(task);
    queue_.Push(task);
  }

 private:
  friend class Worker;

  vector<IThread*> threads_;
  BoundedBlockingQueue<ClosureInterface*> queue_;
}; // class ThreadPool

} // namespace limonp

#endif // LIMONP_THREAD_POOL_HPP
