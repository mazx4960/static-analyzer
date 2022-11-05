// Copyright 2022 CS3203 Team14. All rights reserved.

#include "lexer.h"

#include <iostream>

Lexer::Lexer(std::istream *s) {
  line_number_ = 1;
  column_number_ = 1;
  source_stream_ = s;
  tmp_ = "";
}

char Lexer::Peek() {
  return static_cast<char>(source_stream_->peek());
}
char Lexer::Advance() {
  column_number_++;
  return static_cast<char>(source_stream_->get());
}
/*
 * Skips whitespace characters.
 */
void Lexer::IgnoreWhitespace() {
  std::string s;
  s = Peek();
  while (!source_stream_->eof() && valid_whitespace_.find(s) != valid_whitespace_.end()) {
    if (Peek() == '\n') {
      line_number_++;
      column_number_ = 1;
    }
    Advance();
    s = Peek();
  }
}
/*
 * Ignore comments.
 */
void Lexer::IgnoreComments() {
  while (!source_stream_->eof() && Peek() != '\n') {
    Advance();
  }
}
/*
 * Read digits from the source stream and updates the tmp attribute.
 */
void Lexer::ReadDigits() {
  while (isdigit(Peek())) {
    tmp_ += Advance();
  }
}
/*
 * Read letters or digits from the source stream and updates the tmp attribute.
 */
void Lexer::ReadAlphanumeric() {
  while (isalpha(Peek()) || isdigit(Peek())) {
    this->tmp_ += this->Advance();
  }
}

Token *Lexer::NextToken() {
  IgnoreWhitespace();
  if (source_stream_->eof()) {
    return nullptr;
  }

  char c = Advance();
  tmp_ = c;
  if (isalpha(c) || isdigit(c)) {
    // create symbol token
    ReadAlphanumeric();
    return new SymbolToken(tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number_, column_number_, "Unexpected character: " + tmp_);
}

std::vector<Token *> Lexer::Lex() {
  std::vector<Token *> tokens;
  Token *token = this->NextToken();
  while (token != nullptr) {
    tokens.push_back(token);
    token = NextToken();
  }
  tokens.push_back(new EndOfFileToken());
  return tokens;
}
