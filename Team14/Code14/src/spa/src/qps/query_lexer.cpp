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
  }
  if (isdigit(c)) {
    // Literal
    this->ReadDigits();
    return new LiteralToken(this->tmp_);
  }
  if (c == QueryLexer::kParenthesisOpen) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  }
  if (c == QueryLexer::kParenthesisClose) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  }
  if (c == QueryLexer::kAngularOpen) {
    // Angle Open Bracket
    return new AngleOpenBracketToken();
  }
  if (c == QueryLexer::kAngularClose) {
    // Angle Close Bracket
    return new AngleCloseBracketToken();
  }
  if (c == QueryLexer::kSemicolon) {
    // Semicolon
    return new SemicolonToken();
  }
  if (c == QueryLexer::kDot) {
    // Dot
    return new DotToken();
  }
  if (c == QueryLexer::kComma) {
    // Comma
    return new CommaToken();
  }
  if (c == QueryLexer::kQuote) {
    // Quote
    return new QuoteToken();
  }
  if (c == QueryLexer::kWildCard) {
    // Wild Card
    return new WildCardToken();
  }
  if (c == QueryLexer::kHashtag) {
    // Hashtag
    return new HashtagToken();
  }
  if (QueryKeywords::isValidComparator(tmp_)) {
    // Comparator
    return new ComparatorToken(tmp_);
  }
  if (QueryKeywords::isValidAssignOperator(tmp_)) {
    // Assign Operators
    return new OperatorToken(this->tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(this->line_number_, this->column_number_, "Unexpected character: " + this->tmp_);
}
