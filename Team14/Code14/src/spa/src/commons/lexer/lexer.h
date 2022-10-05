// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <istream>
#include <unordered_set>
#include <vector>

#include "lexer_exceptions.h"
#include "token.h"

class Lexer {
 protected:
  int line_number_;

  int column_number_;

  std::istream *source_stream_;

  std::string tmp_;// temporary string to store a value
  const char semicolon_ = ';';

  const char comma_ = ',';

  const char quote_ = '\"';

  const char round_open_bracket_ = '(';

  const char round_close_bracket_ = ')';

  const char curly_open_bracket_ = '{';

  const char curly_close_bracket_ = '}';

  const char wild_card_ = '_';

  const char hashtag_ = '#';

  std::unordered_set<std::string> valid_whitespace_ = {" ", "\t", "\r", "\n"};

  virtual char peek();
  virtual char advance();
  virtual void ignore_whitespace();
  virtual void ignore_comments();
  virtual void read_alpha();
  virtual void read_digits();
  virtual void read_alphanumeric();

 public:
  explicit Lexer(std::istream *source_stream);
  virtual Token *next_token();
  virtual std::vector<Token *> lex();
};