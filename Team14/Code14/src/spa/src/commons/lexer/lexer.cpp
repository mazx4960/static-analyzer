// Copyright 2022 CS3203 Team14. All rights reserved.

#include "lexer.h"

Lexer::Lexer(std::ifstream *s) {
  line_number_ = 0;
  column_number_ = 0;
  source_stream_ = s;
  tmp_ = "";
}

char Lexer::peek() {
  return static_cast<char>(source_stream_->peek());
}
char Lexer::advance() {
  column_number_++;
  return static_cast<char>(source_stream_->get());
}
/*
 * Skips whitespace characters.
 */
void Lexer::ignore_whitespace() {
  std::string s;
  s = peek();
  while (!source_stream_->eof() && valid_whitespace_.find(s) != valid_whitespace_.end()) {
    if (peek() == '\n') {
      line_number_++;
      column_number_ = 0;
    }
    advance();
    s = peek();
  }
}
/*
 * Read letters from the source stream and updates the tmp attribute.
 */
void Lexer::read_alpha() {
  while (isalpha(peek())) {
    tmp_ += advance();
  }
}
/*
 * Read digits from the source stream and updates the tmp attribute.
 */
void Lexer::read_digits() {
  while (isdigit(peek())) {
    tmp_ += advance();
  }
}
/*
 * Read letters or digits from the source stream and updates the tmp attribute.
 */
void Lexer::read_alphanumeric() {
  while (isalpha(peek()) || isdigit(peek())) {
    this->tmp_ += this->advance();
  }
}

Token *Lexer::next_token() {
  ignore_whitespace();
  if (source_stream_->eof()) {
    return nullptr;
  }

  char c = advance();
  tmp_ = c;
  if (isalpha(c) || isdigit(c)) {
    // Symbol
    read_alphanumeric();
    return new SymbolToken(tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number_, column_number_, "Unexpected character: " + tmp_);
}

std::vector<Token *> Lexer::lex() {
  std::vector<Token *> tokens;
  Token *token = this->next_token();
  while (token) {
    tokens.push_back(token);
    token = next_token();
  }
  tokens.push_back(new EndOfFileToken());
  return tokens;
}
