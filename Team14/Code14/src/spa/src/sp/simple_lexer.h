// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <istream>
#include <unordered_set>
#include <vector>

#include "commons/lexer/lexer.h"
#include "commons/lexer/token.h"

class SimpleLexer : Lexer {
 private:
  std::unordered_set<std::string> valid_single_operators_ = {"+", "-", "*", "/", "%", "<", ">", "&", "|", "!", "="};

  std::unordered_set<std::string> valid_operators_ = {
      "+", "-", "*", "/", "%", "<", ">", ">=", "<=", "&&", "||", "!=", "==", "=", "!"};

  std::unordered_set<std::string> valid_keywords_ = {"procedure", "if", "then", "else",
                                                     "while", "read", "print", "call"};

  void read_operators();

 public:
  explicit SimpleLexer(std::istream *source_stream) : Lexer(source_stream) {
  };
  Token *NextToken() override;
  std::vector<Token *> Lex() override;
};