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

#ifndef CPPJIEBA_PRE_FILTER_H
#define CPPJIEBA_PRE_FILTER_H

#include "Trie.hpp"
#include "limonp/Logging.hpp"

namespace cppjieba {

class PreFilter {
 public:
  //TODO use WordRange instead of Range
  struct Range {
    RuneStrArray::const_iterator begin;
    RuneStrArray::const_iterator end;
  }; // struct Range

  PreFilter(const unordered_set<Rune>& symbols, 
        const string& sentence)
    : symbols_(symbols) {
    if (!DecodeRunesInString(sentence, sentence_)) {
      XLOG(ERROR) << "decode failed. "; 
    }
    cursor_ = sentence_.begin();
  }
  ~PreFilter() {
  }
  bool HasNext() const {
    return cursor_ != sentence_.end();
  }
  Range Next() {
    Range range;
    range.begin = cursor_;
    while (cursor_ != sentence_.end()) {
      if (IsIn(symbols_, cursor_->rune)) {
        if (range.begin == cursor_) {
          cursor_ ++;
        }
        range.end = cursor_;
        return range;
      }
      cursor_ ++;
    }
    range.end = sentence_.end();
    return range;
  }
 private:
  RuneStrArray::const_iterator cursor_;
  RuneStrArray sentence_;
  const unordered_set<Rune>& symbols_;
}; // class PreFilter

} // namespace cppjieba

#endif // CPPJIEBA_PRE_FILTER_H
