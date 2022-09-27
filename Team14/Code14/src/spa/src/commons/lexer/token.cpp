// Copyright 2022 CS3203 Team14. All rights reserved.

#include "token.h"

#include <utility>

Token::Token(TokenType type, std::string value) {
  this->type = type;
  this->value = std::move(value);
}
bool Token::operator==(const Token &other) const {
  return this->type == other.type && this->value == other.value;
}
std::string Token::ToString() const {
  std::string type_string;
  switch (this->type) {
    case TokenType::kSymbol: {
      type_string = "Symbol";
      break;
    }
    case TokenType::kLiteral: {
      type_string = "Literal";
      break;
    }
    case TokenType::kOperator: {
      type_string = "Operator";
      break;
    }
    case TokenType::kQuote: {
      type_string = "Quote";
      break;
    }
    case TokenType::kComma: {
      type_string = "Comma";
      break;
    }
    case TokenType::kSemicolon: {
      type_string = "Semicolon";
      break;
    }
    case TokenType::kRoundOpenBracket: {
      type_string = "RoundOpenBracket";
      break;
    }
    case TokenType::kRoundCloseBracket: {
      type_string = "RoundCloseBracket";
      break;
    }
    case TokenType::kCurlyOpenBracket: {
      type_string = "CurlyOpenBracket";
      break;
    }
    case TokenType::kCurlyCloseBracket: {
      type_string = "CurlyCloseBracket";
      break;
    }
    case TokenType::kEndOfFile: {
      type_string = "EndOfFile";
      break;
    }
    case TokenType::kWildCard: {
      type_string = "WildCard";
      break;
    }
    default: type_string = "unknown token";
  }
  return type_string + "(\"" + this->value + "\")";
}
