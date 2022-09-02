// Copyright 2022 CS3203 Team14. All rights reserved.

#include "lexer.h"
#include "commons/token/token.h"
#include "lexer_exceptions.h"

Lexer::Lexer(std::ifstream* s) {
  line_number = 0;
  column_number = 0;
  source_stream = s;
  tmp = "";
}

char Lexer::peek() {
  return char(source_stream->peek());
}
char Lexer::advance() {
  column_number++;
  return char(source_stream->get());
}
/*
 * Skips whitespace characters.
 */
void Lexer::ignore_whitespace() {
  std::string s;
  s = peek();
  while (!source_stream->eof() && valid_whitespace.find(s) != valid_whitespace.end()) {
    if (peek() == '\n') {
      line_number++;
      column_number = 0;
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
    tmp += advance();
  }
}
/*
 * Read digits from the source stream and updates the tmp attribute.
 */
void Lexer::read_digits() {
  while (isdigit(peek())) {
    tmp += advance();
  }
}
/*
 * Read letters or digits from the source stream and updates the tmp attribute.
 */
void Lexer::read_alphanumeric() {
  while (isalpha(peek()) || isdigit(peek())) {
    this->tmp += this->advance();
  }
}
Token* Lexer::next_token() {
  ignore_whitespace();
  if (source_stream->eof()) {
    return nullptr;
  }

  char c = advance();
  tmp = c;
  if (isalpha(c) || isdigit(c)) {
    // Symbol
    read_alphanumeric();
    return new Token(tmp);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number, column_number, "Unexpected character: " + tmp);
}