// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/token/token.h"

#include <fstream>
#include <unordered_set>
#include <vector>

class Lexer {
 protected:
  int line_number;
  int column_number;
  std::ifstream* source_stream;
  std::string tmp;// temporary string to store a value
  std::unordered_set<std::string> valid_whitespace = {
      " ", "\t", "\r", "\n"};

  virtual char peek();
  virtual char advance();
  virtual void ignore_whitespace();
  virtual void read_alpha();
  virtual void read_digits();
  virtual void read_alphanumeric();

 public:
  explicit Lexer(std::ifstream* source_stream);
  virtual Token* next_token();
  std::vector<Token*> lex();
};