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

#ifndef LIMONP_LOGGING_HPP
#define LIMONP_LOGGING_HPP

#include <sstream>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#ifdef XLOG
#error "XLOG has been defined already"
#endif // XLOG
#ifdef XCHECK
#error "XCHECK has been defined already"
#endif // XCHECK

#define XLOG(level) limonp::Logger(limonp::LL_##level, __FILE__, __LINE__).Stream() 
#define XCHECK(exp) if(!(exp)) XLOG(FATAL) << "exp: ["#exp << "] false. "

namespace limonp {

enum {
  LL_DEBUG = 0, 
  LL_INFO = 1, 
  LL_WARNING = 2, 
  LL_ERROR = 3, 
  LL_FATAL = 4,
}; // enum

static const char * LOG_LEVEL_ARRAY[] = {"DEBUG","INFO","WARN","ERROR","FATAL"};
static const char * LOG_TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

class Logger {
 public:
  Logger(size_t level, const char* filename, int lineno)
   : level_(level) {
#ifdef LOGGING_LEVEL
     if (level_ < LOGGING_LEVEL) {
       return;
     }
#endif
    assert(level_ <= sizeof(LOG_LEVEL_ARRAY)/sizeof(*LOG_LEVEL_ARRAY));
    char buf[32];
    time_t now;
    time(&now);
    strftime(buf, sizeof(buf), LOG_TIME_FORMAT, localtime(&now));
    stream_ << buf 
      << " " << filename 
      << ":" << lineno 
      << " " << LOG_LEVEL_ARRAY[level_] 
      << " ";
  }
  ~Logger() {
#ifdef LOGGING_LEVEL
     if (level_ < LOGGING_LEVEL) {
       return;
     }
#endif
    std::cerr << stream_.str() << std::endl;
    if (level_ == LL_FATAL) {
      abort();
    }
  }

  std::ostream& Stream() {
    return stream_;
  }

 private:
  std::ostringstream stream_;
  size_t level_;
}; // class Logger

} // namespace limonp

#endif // LIMONP_LOGGING_HPP
