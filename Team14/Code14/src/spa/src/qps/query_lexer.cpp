// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_lexer.h"

#include "commons/lexer/lexer_exceptions.h"
#include "qps/pql/query_keywords.h"

Token *QueryLexer::NextToken() {
  this->IgnoreWhitespace();
  if (this->source_stream_->eof()) {
    return nullptr;
  }

  char c = this->Advance();
  this->tmp_ = c;
  if (isalpha(c)) {
    // Keyword or symbol
    this->ReadAlphanumeric();
    if (QueryKeywords::isValidKeyword(tmp_)) {
      return new KeywordToken(this->tmp_);
    }
    return new SymbolToken(this->tmp_);
  } else if (isdigit(c)) {
    // Literal
    this->ReadDigits();
    return new LiteralToken(this->tmp_);
  } else if (c == this->kParenthesisOpen) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  } else if (c == this->kParenthesisClose) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  } else if (c == this->angle_open_bracket_) {
    // Angle Open Bracket
    return new AngleOpenBracketToken();
  } else if (c == this->angle_close_bracket_) {
    // Angle Close Bracket
    return new AngleCloseBracketToken();
  } else if (c == this->kSemicolon) {
    // Semicolon
    return new SemicolonToken();
  } else if (c == this->dot_) {
    // Dot
    return new DotToken();
  } else if (c == this->kComma) {
    // Comma
    return new CommaToken();
  } else if (c == this->kQuote) {
    // Quote
    return new QuoteToken();
  } else if (c == this->wild_card_) {
    // Wild Card
    return new WildCardToken();
  } else if (c == this->kHashtag) {
    // Hashtag
    return new HashtagToken();
  } else if (QueryKeywords::isValidComparator(tmp_)) {
    // Comparator
    return new ComparatorToken(tmp_);
  } else if (QueryKeywords::isValidAssignOperator(tmp_)) {
    // Assign Operators
    return new OperatorToken(this->tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(this->line_number_, this->column_number_, "Unexpected character: " + this->tmp_);
}
