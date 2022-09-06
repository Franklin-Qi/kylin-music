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

#include "gtest/gtest.h"
#include "simple_tokenizer.h"

using namespace simple_tokenizer;

int printFn(void* pCtx, int flags, const char* index, int len, int start, int end) {
  std::cout << "\t" << index << " " << len << " " << start << " " << end << "\n";
  return 0;
}

TEST(simple, tokenizer_with_pinyin) {
  SimpleTokenizer* t = new SimpleTokenizer(nullptr, 0);
  std::vector<std::string> arr;
  std::vector<std::string> query;
  arr.push_back("english@\"''");
  query.push_back(R"VAGON(( e+n+g+l+i+s+h* OR eng+li+sh* OR english* ) AND "@" AND """" AND "'" AND "'")VAGON");
  arr.push_back("zhou杰伦");
  query.push_back(R"VAGON(( z+h+o+u* OR zhou* ) AND "杰" AND "伦")VAGON");
  arr.push_back("杰伦 zhou 123");
  query.push_back(R"VAGON("杰" AND "伦" AND ( z+h+o+u* OR zhou* ) AND "123"*)VAGON");
  for (int i = 0; i < arr.size(); i++) {
    std::string s = arr[i];
    std::cout << s << " as doc:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_DOCUMENT, s.c_str(), s.length(), printFn);
    std::cout << s << " as query:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_QUERY, s.c_str(), s.length(), printFn);
    std::cout << s << " as aux:\n";
    std::string result = SimpleTokenizer::tokenize_query(s.c_str(), s.length());
    std::cout << result << "\n";
    ASSERT_EQ(result, query[i]);
  }
}

TEST(simple, tokenizer_disable_pinyin) {
  const char* p = "0";
  SimpleTokenizer* t = new SimpleTokenizer(&p, 1);
  std::vector<std::string> arr;
  std::vector<std::string> query;
  arr.push_back("english@\"''");
  query.push_back(R"VAGON(english* AND "@" AND """" AND "'" AND "'")VAGON");
  arr.push_back("zhou杰伦");
  query.push_back(R"VAGON(zhou* AND "杰" AND "伦")VAGON");
  arr.push_back("杰伦123");
  query.push_back(R"VAGON("杰" AND "伦" AND "123"*)VAGON");
  for (int i = 0; i < arr.size(); i++) {
    std::string s = arr[i];
    std::cout << s << " as doc:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_DOCUMENT, s.c_str(), s.length(), printFn);
    std::cout << s << " as query:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_QUERY, s.c_str(), s.length(), printFn);
    std::cout << s << " as aux:\n";
    std::string result = SimpleTokenizer::tokenize_query(s.c_str(), s.length(), 0);
    std::cout << result << "\n";
    ASSERT_EQ(result, query[i]);
  }
}

#ifdef USE_JIEBA
TEST(simple, jieba_tokenizer_with_pinyin) {
  SimpleTokenizer* t = new SimpleTokenizer(nullptr, 0);
  std::vector<std::string> arr;
  std::vector<std::string> query;
  arr.push_back("english@\"''");
  query.push_back(R"VAGON(( e+n+g+l+i+s+h* OR eng+li+sh* OR english* ) AND "@" AND """" AND "'" AND "'")VAGON");
  arr.push_back("zhou杰伦");
  query.push_back(R"VAGON(( z+h+o+u* OR zhou* ) AND "杰伦")VAGON");
  arr.push_back("杰伦 zhou 123");
  query.push_back(R"VAGON("杰伦" AND ( z+h+o+u* OR zhou* ) AND "123"*)VAGON");
  for (int i = 0; i < arr.size(); i++) {
    std::string s = arr[i];
    std::cout << s << " as doc:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_DOCUMENT, s.c_str(), s.length(), printFn);
    std::cout << s << " as query:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_QUERY, s.c_str(), s.length(), printFn);
    std::cout << s << " as aux:\n";
    std::string result = SimpleTokenizer::tokenize_jieba_query(s.c_str(), s.length());
    std::cout << result << "\n";
    ASSERT_EQ(result, query[i]);
  }
}

TEST(simple, jieba_tokenizer_disable_pinyin) {
  const char* p = "0";
  SimpleTokenizer* t = new SimpleTokenizer(&p, 1);
  std::vector<std::string> arr;
  std::vector<std::string> query;
  arr.push_back("english@\"''");
  query.push_back(R"VAGON(english* AND "@" AND """" AND "'" AND "'")VAGON");
  arr.push_back("zhou杰伦");
  query.push_back(R"VAGON(zhou* AND "杰伦")VAGON");
  arr.push_back("杰伦123");
  query.push_back(R"VAGON("杰伦" AND "123"*)VAGON");
  for (int i = 0; i < arr.size(); i++) {
    std::string s = arr[i];
    std::cout << s << " as doc:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_DOCUMENT, s.c_str(), s.length(), printFn);
    std::cout << s << " as query:\n";
    t->tokenize(nullptr, FTS5_TOKENIZE_QUERY, s.c_str(), s.length(), printFn);
    std::cout << s << " as aux:\n";
    std::string result = SimpleTokenizer::tokenize_jieba_query(s.c_str(), s.length(), 0);
    std::cout << result << "\n";
    ASSERT_EQ(result, query[i]);
  }
}
#endif
