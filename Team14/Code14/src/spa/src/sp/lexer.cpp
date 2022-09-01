// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/lexer.h"

#include "sp/exceptions.h"
#include "sp/token.h"

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
 * Read operators from the source stream and updates the tmp attribute.
 */
void Lexer::read_operators() {
  std::string s;
  s = peek();
  while (valid_single_operators.find(s) != valid_whitespace.end()) {
    tmp += advance();
  }
}
Token* Lexer::next_token() {
  ignore_whitespace();
  if (source_stream->eof()) {
    return nullptr;
  }

  char c = advance();
  tmp = c;
  if (isalpha(c)) {
    // Symbol or keyword
    read_alpha();
    if (valid_keywords.find(tmp) != valid_whitespace.end()) {
      return new KeywordToken(tmp);
    } else {
      return new SymbolToken(tmp);
    }
  } else if (isdigit(c)) {
    // Literal
    read_digits();
    return new LiteralToken(tmp);
  } else if (valid_single_operators.find(tmp) != valid_whitespace.end()) {
    // Operator
    read_operators();
    if (valid_operators.find(tmp) != valid_whitespace.end()) {
      return new OperatorToken(tmp);
    } else {
      throw LexSyntaxError(line_number, column_number, "Invalid operator " + tmp);
    }
    return new OperatorToken(tmp);
  } else if (valid_separators.find(tmp) != valid_whitespace.end()) {
    // Separator
    return new SeparatorToken(tmp);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number, column_number, "Unexpected character: " + tmp);
}
std::vector<Token*> Lexer::lex() {
  std::vector<Token*> tokens;
  Token* token = next_token();
  while (token) {
    tokens.push_back(token);
    token = next_token();
  }
  tokens.push_back(new EndOfFileToken());
  return tokens;
}
