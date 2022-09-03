// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/simple_lexer.h"
#include "commons/lexer/lexer_exceptions.h"

/*
 * Read operators from the source stream and updates the tmp attribute.
 */
void SimpleLexer::read_operators() {
  std::string s;
  s = peek();
  while (valid_single_operators_.find(s) != valid_whitespace_.end()) {
    tmp_ += advance();
  }
}
Token* SimpleLexer::next_token() {
  ignore_whitespace();
  if (source_stream_->eof()) {
    return nullptr;
  }

  char c = advance();
  tmp_ = c;
  if (isalpha(c)) {
    // Symbol or keyword
    read_alphanumeric();
    if (valid_keywords_.find(tmp_) != valid_whitespace_.end()) {
      return new KeywordToken(tmp_);
    }
    return new SymbolToken(tmp_);
  } else if (isdigit(c)) {
    // Literal
    read_digits();
    return new LiteralToken(tmp_);
  } else if (c == this->semicolon_) {
    // Semicolon
    return new SemicolonToken();
  } else if (c == this->round_open_bracket_) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  } else if (c == this->round_close_bracket_) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  } else if (c == this->curly_open_bracket_) {
    // Curly Open Bracket
    return new CurlyOpenBracketToken();
  } else if (c == this->curly_close_bracket_) {
    // Curly Close Bracket
    return new CurlyCloseBracket();
  } else if (valid_single_operators_.find(tmp_) != valid_whitespace_.end()) {
    // Operator
    read_operators();
    if (valid_operators_.find(tmp_) != valid_whitespace_.end()) {
      return new OperatorToken(tmp_);
    }
    throw LexSyntaxError(line_number_, column_number_, "Invalid operator " + tmp_);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number_, column_number_, "Unexpected character: " + tmp_);
}
