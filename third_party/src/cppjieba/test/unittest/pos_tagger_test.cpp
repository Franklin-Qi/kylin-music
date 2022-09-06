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

#include "cppjieba/MixSegment.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;

static const char * const QUERY_TEST1 = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。";
static const char * const ANS_TEST1 = "[我:r, 是:v, 蓝翔:x, 技工:n, 拖拉机:n, 学院:n, 手扶拖拉机:n, 专业:n, 的:uj, 。:x, 不用:v, 多久:m, ，:x, 我:r, 就:d, 会:v, 升职:v, 加薪:nr, ，:x, 当上:t, 总经理:n, ，:x, 出任:v, CEO:eng, ，:x, 迎娶:v, 白富:x, 美:ns, ，:x, 走上:v, 人生:n, 巅峰:n, 。:x]";
static const char * const QUERY_TEST2 = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。";
static const char * const ANS_TEST2 = "[我:r, 是:v, 蓝翔:nz, 技工:n, 拖拉机:n, 学院:n, 手扶拖拉机:n, 专业:n, 的:uj, 。:x, 不用:v, 多久:m, ，:x, 我:r, 就:d, 会:v, 升职:v, 加薪:nr, ，:x, 当上:t, 总经理:n, ，:x, 出任:v, CEO:eng, ，:x, 迎娶:v, 白富:x, 美:ns, ，:x, 走上:v, 人生:n, 巅峰:n, 。:x]";

static const char * const QUERY_TEST3 = "iPhone6手机的最大特点是很容易弯曲。";
static const char * const ANS_TEST3 = "[iPhone6:eng, 手机:n, 的:uj, 最大:a, 特点:n, 是:v, 很:zg, 容易:a, 弯曲:v, 。:x]";
//static const char * const ANS_TEST3 = "";

TEST(PosTaggerTest, Test) {
  MixSegment tagger("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8");
  {
    vector<pair<string, string> > res;
    tagger.Tag(QUERY_TEST1, res);
    string s;
    s << res;
    ASSERT_TRUE(s == ANS_TEST1);
  }
}
TEST(PosTagger, TestUserDict) {
  MixSegment tagger("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../test/testdata/userdict.utf8");
  {
    vector<pair<string, string> > res;
    tagger.Tag(QUERY_TEST2, res);
    string s;
    s << res;
    ASSERT_EQ(s, ANS_TEST2);
  }
  {
    vector<pair<string, string> > res;
    tagger.Tag(QUERY_TEST3, res);
    string s;
    s << res;
    ASSERT_EQ(s, ANS_TEST3);
  }
}
