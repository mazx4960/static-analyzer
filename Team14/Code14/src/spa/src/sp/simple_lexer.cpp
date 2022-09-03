// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp/simple_lexer.h"
#include "commons/lexer/lexer_exceptions.h"

/*
 * Read operators from the source stream and updates the tmp attribute.
 */
void SimpleLexer::read_operators() {
  std::string s;
  s = peek();
  while (valid_single_operators.find(s) != valid_whitespace.end()) {
    tmp += advance();
  }
}
Token* SimpleLexer::next_token() {
  ignore_whitespace();
  if (source_stream->eof()) {
    return nullptr;
  }

  char c = advance();
  tmp = c;
  if (isalpha(c)) {
    // Symbol or keyword
    read_alphanumeric();
    if (valid_keywords.find(tmp) != valid_whitespace.end()) {
      return new KeywordToken(tmp);
    } else {
      return new SymbolToken(tmp);
    }
  } else if (isdigit(c)) {
    // Literal
    read_digits();
    return new LiteralToken(tmp);
  } else if (c == this->semicolon) {
    // Semicolon
    return new SemicolonToken();
  } else if (c == this->round_open_bracket) {
    // Round Open Bracket
    return new RoundOpenBracketToken();
  } else if (c == this->round_close_bracket) {
    // Round Close Bracket
    return new RoundCloseBracketToken();
  } else if (c == this->curly_open_bracket) {
    // Curly Open Bracket
    return new CurlyOpenBracketToken();
  } else if (c == this->curly_close_bracket) {
    // Curly Close Bracket
    return new CurlyCloseBracket();
  } else if (valid_single_operators.find(tmp) != valid_whitespace.end()) {
    // Operator
    read_operators();
    if (valid_operators.find(tmp) != valid_whitespace.end()) {
      return new OperatorToken(tmp);
    } else {
      throw LexSyntaxError(line_number, column_number, "Invalid operator " + tmp);
    }
    return new OperatorToken(tmp);
  }

  // Something went wrong :/
  throw LexSyntaxError(line_number, column_number, "Unexpected character: " + tmp);
}
