// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_lexer.h"
#include "query_lexer_exceptions.h"

Token* QueryLexer::next_token() {
  this->ignore_whitespace();
  if (this->source_stream_->eof()) {
    return nullptr;
  }

  char c = this->advance();
  this->tmp_ = c;
  if (isalpha(c)) {
    // Keyword or symbol
    this->read_alphanumeric();
    if (this->valid_keywords_.find(this->tmp_) != this->valid_whitespace_.end()) {
      return new KeywordToken(this->tmp_);
    }
    return new SymbolToken(this->tmp_);
  } else if (isdigit(c)) {
    // Literal
    this->read_digits();
    return new LiteralToken(this->tmp_);
  } else if (c == this->round_open_bracket_) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  } else if (c == this->round_close_bracket_) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  } else if (c == this->semicolon_) {
    // Semicolon
    return new SemicolonToken();
  } else if (c == this->comma_) {
    // Comma
    return new CommaToken();
  } else if (c == this->quote_) {
    // Quote
    return new QuoteToken();
  } else if (this->valid_assign_operators_.find(this->tmp_) != this->valid_whitespace_.end()) {
    // Assign Operators
    return new OperatorToken(this->tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(this->line_number_, this->column_number_, "Unexpected character: " + this->tmp_);
}
