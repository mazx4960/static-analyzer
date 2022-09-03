// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/lexer/lexer.h"
#include "commons/token/token.h"

#include <fstream>
#include <unordered_set>
#include <vector>

class QueryLexer: public Lexer {
 private:
  std::unordered_set<std::string> valid_assign_operators_ = {
      "+", "-", "*", "/", "%", "<", ">"};
  std::unordered_set<std::string> valid_keywords_ = {
      "Select", "such", "that", "Follows", "Parent", "Uses",
      "Modifies", "pattern", "stmt", "read", "print", "call", "while",
      "if", "assign", "variable", "constant", "procedure"};

 public:
  explicit QueryLexer(std::ifstream* source_stream) : Lexer(source_stream) {};
  Token* next_token() override;
};