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

#ifndef LIMONP_FILELOCK_HPP
#define LIMONP_FILELOCK_HPP

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <assert.h>

namespace limonp {

using std::string;

class FileLock {
 public:
  FileLock() : fd_(-1), ok_(true) {
  }
  ~FileLock() {
    if(fd_ > 0) {
      Close();
    }
  }
  void Open(const string& fname) {
    assert(fd_ == -1);
    fd_ = open(fname.c_str(), O_RDWR | O_CREAT, 0644);
    if(fd_ < 0) {
      ok_ = false;
      err_ = strerror(errno);
    }
  }
  void Close() {
    ::close(fd_);
  }
  void Lock() {
    if(LockOrUnlock(fd_, true) < 0) {
      ok_ = false;
      err_ = strerror(errno);
    }
  }
  void UnLock() {
    if(LockOrUnlock(fd_, false) < 0) {
      ok_ = false;
      err_ = strerror(errno);
    }
  }
  bool Ok() const {
    return ok_;
  }
  string Error() const {
    return err_;
  }
 private:
  static int LockOrUnlock(int fd, bool lock) {
    errno = 0;
    struct flock f;
    memset(&f, 0, sizeof(f));
    f.l_type = (lock ? F_WRLCK : F_UNLCK);
    f.l_whence = SEEK_SET;
    f.l_start = 0;
    f.l_len = 0;        // Lock/unlock entire file
    return fcntl(fd, F_SETLK, &f);
  }

  int fd_;
  bool ok_;
  string err_;
}; // class FileLock

}// namespace limonp

#endif // LIMONP_FILELOCK_HPP
