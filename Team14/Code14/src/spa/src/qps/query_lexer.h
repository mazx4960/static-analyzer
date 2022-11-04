// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <istream>
#include <unordered_set>
#include <vector>

#include "commons/lexer/lexer.h"
#include "commons/lexer/token.h"

class QueryLexer : public Lexer {
 protected:
  const char dot_ = '.';

  const char angle_open_bracket_ = '<';

  const char angle_close_bracket_ = '>';

  const char wild_card_ = '_';

 public:
  explicit QueryLexer(std::istream *source_stream) : Lexer(source_stream) {
  };
  Token *NextToken() override;
};