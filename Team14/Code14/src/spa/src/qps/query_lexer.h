// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <istream>
#include <unordered_set>
#include <vector>

#include "commons/lexer/lexer.h"
#include "commons/lexer/token.h"

class QueryLexer : public Lexer {
 protected:
  static const char kDot = '.';

  static const char kAngularOpen = '<';

  static const char kAngularClose = '>';

  static const char kWildCard = '_';

 public:
  explicit QueryLexer(std::istream *source_stream) : Lexer(source_stream) {
  };
  Token *NextToken() override;
};