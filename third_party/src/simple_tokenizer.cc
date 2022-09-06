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

#include "simple_tokenizer.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace simple_tokenizer {
SimpleTokenizer::SimpleTokenizer(const char **azArg, int nArg) {
  if (nArg >= 1) {
    enable_pinyin = atoi(azArg[0]) != 0;
  }
}

PinYin *SimpleTokenizer::get_pinyin() {
  static auto *py = new PinYin();
  return py;
}

static TokenCategory from_char(char c) {
  if (std::isdigit(c)) {
    return TokenCategory::DIGIT;
  }
  if (std::isspace(c)) {
    return TokenCategory::SPACE;
  }
  if (std::isalpha(c)) {
    return TokenCategory::ASCII_ALPHABETIC;
  }
  return TokenCategory::OTHER;
}

std::string SimpleTokenizer::tokenize_query(const char *text, int textLen, int flags) {
  int start = 0;
  int index = 0;
  std::string tmp;
  std::string result;
  while (index < textLen) {
    TokenCategory category = from_char(text[index]);
    switch (category) {
      case TokenCategory::OTHER:
        index += PinYin::get_str_len(text[index]);
        break;
      default:
        while (++index < textLen && from_char(text[index]) == category) {
        }
        break;
    }
    tmp.clear();
    std::copy(text + start, text + index, std::back_inserter(tmp));
    append_result(result, tmp, category, start, flags);
    start = index;
  }
  return result;
}

#ifdef USE_JIEBA
std::string jieba_dict_path = "./dict/";
std::string SimpleTokenizer::tokenize_jieba_query(const char *text, int textLen, int flags) {
  (void)textLen;
  static cppjieba::Jieba jieba(jieba_dict_path + "jieba.dict.utf8", jieba_dict_path + "hmm_model.utf8",
                               jieba_dict_path + "user.dict.utf8", jieba_dict_path + "idf.utf8",
                               jieba_dict_path + "stop_words.utf8");
  std::string tmp;
  std::string result;
  std::vector<cppjieba::Word> words;
  jieba.Cut(text, words);
  for (auto word : words) {
    TokenCategory category = from_char(text[word.offset]);
    append_result(result, word.word, category, word.offset, flags);
  }
  return result;
}
#endif

void SimpleTokenizer::append_result(std::string &result, std::string part, TokenCategory category, int offset,
                                    int flags) {
  if (category != TokenCategory::SPACE) {
    std::string tmp = std::move(part);
    if (category == TokenCategory::ASCII_ALPHABETIC) {
      std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    if (flags != 0 && category == TokenCategory::ASCII_ALPHABETIC && tmp.size() > 1) {
      if (offset == 0) {
        result.append("( ");
      } else {
        result.append(" AND ( ");
      }
      std::set<std::string> pys = SimpleTokenizer::get_pinyin()->split_pinyin(tmp);
      bool addOr = false;
      for (const std::string &s : pys) {
        if (addOr) {
          result.append(" OR ");
        }
        result.append(s);
        result.append("*");
        addOr = true;
      }
      result.append(" )");
    } else {
      if (offset > 0) {
        result.append(" AND ");
      }
      if (tmp == "\"") {
        tmp += tmp;
      }
      if (category != TokenCategory::ASCII_ALPHABETIC) {
        result.append('"' + tmp + '"');
      } else {
        result.append(tmp);
      }
      if (category != TokenCategory::OTHER) {
        result.append("*");
      }
    }
  }
}

// https://cloud.tencent.com/developer/article/1198371
int SimpleTokenizer::tokenize(void *pCtx, int flags, const char *text, int textLen, xTokenFn xToken) const {
  int rc = SQLITE_OK;
  int start = 0;
  int index = 0;
  std::string result;
  while (index < textLen) {
    TokenCategory category = from_char(text[index]);
    switch (category) {
      case TokenCategory::OTHER:
        index += PinYin::get_str_len(text[index]);
        break;
      default:
        while (++index < textLen && from_char(text[index]) == category) {
        }
        break;
    }
    if (category != TokenCategory::SPACE) {
      result.clear();
      std::copy(text + start, text + index, std::back_inserter(result));
      if (category == TokenCategory::ASCII_ALPHABETIC) {
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
      }

      rc = xToken(pCtx, 0, result.c_str(), (int)result.length(), start, index);
      if (enable_pinyin && category == TokenCategory::OTHER && (flags & FTS5_TOKENIZE_DOCUMENT)) {
        const std::vector<std::string> &pys = SimpleTokenizer::get_pinyin()->get_pinyin(result);
        for (const std::string &s : pys) {
          rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, s.c_str(), (int)s.length(), start, index);
        }
      }
    }
    start = index;
  }
  return rc;
}
}  // namespace simple_tokenizer
