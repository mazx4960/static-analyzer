// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <istream>
#include <unordered_set>
#include <vector>

#include "lexer_exceptions.h"
#include "token.h"

class Lexer {
 protected:
  static const char kSemicolon = ';';

  static const char kComma = ',';

  static const char kQuote = '\"';

  static const char kParenthesisOpen = '(';

  static const char kParenthesisClose = ')';

  static const char kBraceOpen = '{';

  static const char kBraceClose = '}';

  static const char kHashtag = '#';

  int line_number_;

  int column_number_;

  std::istream *source_stream_;

  std::string tmp_;   // temporary string to store a value

  std::unordered_set<std::string> valid_whitespace_ = {" ", "\t", "\r", "\n"};

  virtual char Peek();
  virtual char Advance();
  virtual void IgnoreWhitespace();
  virtual void IgnoreComments();
  virtual void ReadDigits();
  virtual void ReadAlphanumeric();

 public:
  explicit Lexer(std::istream *source_stream);
  virtual Token *NextToken();
  virtual std::vector<Token *> Lex();
};