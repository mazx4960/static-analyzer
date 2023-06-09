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
bool Token::operator!=(const Token &other) const {
  return !(*this == other);
}
std::string Token::ToString() const {
  std::string type_string = TokenTypeToString(this->type);
  return type_string + "(\"" + this->value + "\")";
}
