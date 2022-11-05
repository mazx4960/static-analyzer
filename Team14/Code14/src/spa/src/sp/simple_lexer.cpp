// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/simple_lexer.h"

#include "commons/lexer/lexer_exceptions.h"

/*
 * Read operators from the source stream and updates the tmp attribute.
 */
void SimpleLexer::read_operators() {
  std::string s;
  s = Peek();
  while (valid_single_operators_.find(s) != valid_single_operators_.end()) {
    tmp_ += Advance();
    s = Peek();
  }
}
Token *SimpleLexer::NextToken() {
  IgnoreWhitespace();
  if (source_stream_->eof()) {
    return nullptr;
  }

  char c = Advance();
  tmp_ = c;
  if (c == kHashtag) {
    IgnoreComments();
    return NextToken();
  }
  if (isalpha(c)) {
    // Symbol or keyword
    ReadAlphanumeric();
    if (valid_keywords_.find(tmp_) != valid_keywords_.end()) {
      return new KeywordToken(tmp_);
    }
    return new SymbolToken(tmp_);
  }
  if (isdigit(c)) {
    // Literal
    ReadDigits();
    return new LiteralToken(tmp_);
  }
  if (c == SimpleLexer::kSemicolon) {
    // Semicolon
    return new SemicolonToken();
  }
  if (c == SimpleLexer::kParenthesisOpen) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  }
  if (c == SimpleLexer::kParenthesisClose) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  }
  if (c == SimpleLexer::kBraceOpen) {
    // Curly Open Bracket
    return new CurlyOpenBracketToken();
  }
  if (c == SimpleLexer::kBraceClose) {
    // Curly Close Bracket
    return new CurlyCloseBracketToken();
  }
  if (valid_single_operators_.find(tmp_) != valid_single_operators_.end()) {
    // Operator
    read_operators();
    if (valid_operators_.find(tmp_) == valid_operators_.end()) {
      throw LexSyntaxError(line_number_, column_number_, "Invalid operator " + tmp_);
    }
    return new OperatorToken(tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number_, column_number_, "Unexpected character: " + tmp_);
}
std::vector<Token *> SimpleLexer::Lex() {
  return Lexer::Lex();
}
