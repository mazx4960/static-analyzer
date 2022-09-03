// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_lexer.h"
#include "query_lexer_exceptions.h"

Token* QueryLexer::next_token() {
  this->ignore_whitespace();
  if (this->source_stream->eof()) {
    return nullptr;
  }

  char c = this->advance();
  this->tmp = c;
  if (isalpha(c)) {
    // Keyword or symbol
    this->read_alpha();
    if (this->valid_keywords.find(this->tmp) != this->valid_whitespace.end()) {
      return new KeywordToken(this->tmp);
    } else {
      this->read_alphanumeric();
      return new SymbolToken(this->tmp);
    }
  } else if (isdigit(c)) {
    // Literal
    this->read_digits();
    return new LiteralToken(this->tmp);
  } else if (this->valid_assign_operators.find(this->tmp) != this->valid_whitespace.end()) {
    // Assign Operator
    return new OperatorToken(this->tmp);
  } else if (this->valid_separators.find(this->tmp) != this->valid_whitespace.end()) {
    // Separator
    return new SeparatorToken(this->tmp);
  }

  // Something went wrong :/
  throw LexSyntaxError(this->line_number, this->column_number, "Unexpected character: " + this->tmp);
}
std::vector<Token*> QueryLexer::lex() {
  std::vector<Token*> tokens;
  Token* token = next_token();
  while (token) {
    tokens.push_back(token);
    token = next_token();
  }
  tokens.push_back(new EndOfFileToken());
  return tokens;
}
