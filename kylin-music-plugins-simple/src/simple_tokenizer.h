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

#ifndef SIMPLE_TOKENIZER_H_
#define SIMPLE_TOKENIZER_H_

#include <memory>

#ifdef USE_JIEBA
#include "cppjieba/Jieba.hpp"
#endif
#include "pinyin.h"
#include "sqlite3ext.h"

typedef int (*xTokenFn)(void *, int, const char *, int, int, int);

namespace simple_tokenizer {

#ifdef USE_JIEBA
extern std::string jieba_dict_path;
#endif

enum class TokenCategory {
  SPACE,
  ASCII_ALPHABETIC,
  DIGIT,
  OTHER,
};

class SimpleTokenizer {
 private:
  static PinYin *get_pinyin();
  bool enable_pinyin = true;

 public:
  SimpleTokenizer(const char **zaArg, int nArg);
  int tokenize(void *pCtx, int flags, const char *text, int textLen, xTokenFn xToken) const;
  static std::string tokenize_query(const char *text, int textLen, int flags = 1);
#ifdef USE_JIEBA
  static std::string tokenize_jieba_query(const char *text, int textLen, int flags = 1);
#endif

 private:
  static void append_result(std::string &result, std::string part, TokenCategory category, int offset, int flags);
};

}  // namespace simple_tokenizer

extern "C" int fts5_simple_xCreate(void *sqlite3, const char **azArg, int nArg, Fts5Tokenizer **ppOut);
extern "C" int fts5_simple_xTokenize(Fts5Tokenizer *tokenizer_ptr, void *pCtx, int flags, const char *pText, int nText,
                                     xTokenFn xToken);
extern "C" void fts5_simple_xDelete(Fts5Tokenizer *tokenizer_ptr);

extern "C" int sqlite3_simple_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

#endif  // SIMPLE_TOKENIZER_H_
