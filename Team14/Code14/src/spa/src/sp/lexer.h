// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <fstream>
#include <unordered_set>
#include <vector>

#include "sp/token.h"

class Lexer {
 private:
  int line_number;
  int column_number;
  std::ifstream* source_stream;
  std::string tmp;  // temporary string to store a value
  std::unordered_set<std::string> valid_whitespace = {" ", "\t", "\r", "\n"};
  std::unordered_set<std::string> valid_single_operators = {
      "+", "-", "*", "/", "%", "<", ">", "&", "|", "!", "="};
  std::unordered_set<std::string> valid_operators = {
      "+",  "-",  "*",  "/",  "%",  "<",  ">",
      ">=", "<=", "&&", "||", "!=", "==", "="};
  std::unordered_set<std::string> valid_separators = {";", "(", ")", "{", "}"};
  std::unordered_set<std::string> valid_keywords = {
      "procedure", "if", "then", "else", "while", "read", "print", "call"};

  char peek();
  char advance();
  void ignore_whitespace();
  void read_alpha();
  void read_digits();
  void read_operators();

 public:
  Lexer(std::ifstream* source_stream);
  Token* next_token();
  std::vector<Token*> lex();
};