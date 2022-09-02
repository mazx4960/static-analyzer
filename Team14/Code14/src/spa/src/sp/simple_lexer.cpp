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
SimpleToken* SimpleLexer::next_token() {
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
      read_alphanumeric();
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
std::vector<SimpleToken*> SimpleLexer::lex() {
  std::vector<SimpleToken*> tokens;
  SimpleToken* token = next_token();
  while (token) {
    tokens.push_back(token);
    token = next_token();
  }
  tokens.push_back(new EndOfFileToken());
  return tokens;
}
