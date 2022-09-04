// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>
#include <unordered_set>
#include <vector>

#include "commons/lexer/lexer.h"
#include "commons/token/token.h"

class SimpleLexer : Lexer {
 private:

  std::unordered_set<std::string> valid_single_operators_ = {
      "+", "-", "*", "/", "%", "<", ">", "&", "|", "!", "="};
  std::unordered_set<std::string> valid_operators_ = {
      "+", "-", "*", "/", "%", "<", ">", ">=", "<=", "&&", "||", "!=", "==", "=", "!"};
  std::unordered_set<std::string> valid_keywords_ = {
      "procedure", "if", "then", "else", "while", "read", "print", "call"};

  void read_operators();

 public:
  explicit SimpleLexer(std::ifstream* source_stream) : Lexer(source_stream) {};
  Token* next_token() override;
};