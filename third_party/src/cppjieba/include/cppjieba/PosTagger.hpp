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

#ifndef CPPJIEBA_POS_TAGGING_H
#define CPPJIEBA_POS_TAGGING_H

#include "limonp/StringUtil.hpp"
#include "SegmentTagged.hpp"
#include "DictTrie.hpp"

namespace cppjieba {
using namespace limonp;

static const char* const POS_M = "m";
static const char* const POS_ENG = "eng";
static const char* const POS_X = "x";

class PosTagger {
 public:
  PosTagger() {
  }
  ~PosTagger() {
  }

  bool Tag(const string& src, vector<pair<string, string> >& res, const SegmentTagged& segment) const {
    vector<string> CutRes;
    segment.Cut(src, CutRes);

    for (vector<string>::iterator itr = CutRes.begin(); itr != CutRes.end(); ++itr) {
      res.push_back(make_pair(*itr, LookupTag(*itr, segment)));
    }
    return !res.empty();
  }

  string LookupTag(const string &str, const SegmentTagged& segment) const {
    const DictUnit *tmp = NULL;
    RuneStrArray runes;
    const DictTrie * dict = segment.GetDictTrie();
    assert(dict != NULL);
      if (!DecodeRunesInString(str, runes)) {
        XLOG(ERROR) << "Decode failed.";
        return POS_X;
      }
      tmp = dict->Find(runes.begin(), runes.end());
      if (tmp == NULL || tmp->tag.empty()) {
        return SpecialRule(runes);
      } else {
        return tmp->tag;
      }
  }

 private:
  const char* SpecialRule(const RuneStrArray& unicode) const {
    size_t m = 0;
    size_t eng = 0;
    for (size_t i = 0; i < unicode.size() && eng < unicode.size() / 2; i++) {
      if (unicode[i].rune < 0x80) {
        eng ++;
        if ('0' <= unicode[i].rune && unicode[i].rune <= '9') {
          m++;
        }
      }
    }
    // ascii char is not found
    if (eng == 0) {
      return POS_X;
    }
    // all the ascii is number char
    if (m == eng) {
      return POS_M;
    }
    // the ascii chars contain english letter
    return POS_ENG;
  }

}; // class PosTagger

} // namespace cppjieba

#endif
