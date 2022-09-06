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

/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
 ************************************/

#ifndef LIMONP_ARGV_FUNCTS_H
#define LIMONP_ARGV_FUNCTS_H

#include <set>
#include <sstream>
#include "StringUtil.hpp"

namespace limonp {

using namespace std;

class ArgvContext {
 public :
  ArgvContext(int argc, const char* const * argv) {
    for(int i = 0; i < argc; i++) {
      if(StartsWith(argv[i], "-")) {
        if(i + 1 < argc && !StartsWith(argv[i + 1], "-")) {
          mpss_[argv[i]] = argv[i+1];
          i++;
        } else {
          sset_.insert(argv[i]);
        }
      } else {
        args_.push_back(argv[i]);
      }
    }
  }
  ~ArgvContext() {
  }

  friend ostream& operator << (ostream& os, const ArgvContext& args);
  string operator [](size_t i) const {
    if(i < args_.size()) {
      return args_[i];
    }
    return "";
  }
  string operator [](const string& key) const {
    map<string, string>::const_iterator it = mpss_.find(key);
    if(it != mpss_.end()) {
      return it->second;
    }
    return "";
  }

  bool HasKey(const string& key) const {
    if(mpss_.find(key) != mpss_.end() || sset_.find(key) != sset_.end()) {
      return true;
    }
    return false;
  }

 private:
  vector<string> args_;
  map<string, string> mpss_;
  set<string> sset_;
}; // class ArgvContext

inline ostream& operator << (ostream& os, const ArgvContext& args) {
  return os<<args.args_<<args.mpss_<<args.sset_;
}

} // namespace limonp

#endif
