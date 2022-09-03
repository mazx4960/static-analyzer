// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>
#include <unordered_set>
#include <vector>

#include "commons/lexer/lexer.h"
#include "commons/token/token.h"

class SimpleLexer : Lexer {
 private:
  char semicolon = ';';
  char round_open_bracket = '(';
  char round_close_bracket = ')';
  char curly_open_bracket = '{';
  char curly_close_bracket = '}';
  std::unordered_set<std::string> valid_single_operators = {
      "+", "-", "*", "/", "%", "<", ">", "&", "|", "!", "="};
  std::unordered_set<std::string> valid_operators = {
      "+", "-", "*", "/", "%", "<", ">", ">=", "<=", "&&", "||", "!=", "==", "="};
  std::unordered_set<std::string> valid_keywords = {
      "procedure", "if", "then", "else", "while", "read", "print", "call"};

  void read_operators();

 public:
  explicit SimpleLexer(std::ifstream* source_stream) : Lexer(source_stream) {};
  Token* next_token() override;
};