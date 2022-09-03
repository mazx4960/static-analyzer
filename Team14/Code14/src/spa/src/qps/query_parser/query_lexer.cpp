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
    this->read_alphanumeric();
    if (this->valid_keywords.find(this->tmp) != this->valid_whitespace.end()) {
      return new KeywordToken(this->tmp);
    } else {
      return new SymbolToken(this->tmp);
    }
  } else if (isdigit(c)) {
    // Literal
    this->read_digits();
    return new LiteralToken(this->tmp);
  } else if (c == this->round_open_bracket) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  } else if (c == this->round_close_bracket) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  } else if (c == this->semicolon) {
    // Semicolon
    return new SemicolonToken();
  } else if (c == this->comma) {
    // Comma
    return new CommaToken();
  } else if (c == this->quote) {
    // Quote
    return new QuoteToken();
  } else if (this->valid_assign_operators.find(this->tmp) != this->valid_whitespace.end()) {
    // Assign Operators
    return new OperatorToken(this->tmp);
  }

  // Something went wrong :/
  throw LexSyntaxError(this->line_number, this->column_number, "Unexpected character: " + this->tmp);
}

