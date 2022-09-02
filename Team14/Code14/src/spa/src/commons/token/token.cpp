// Copyright 2022 CS3203 Team14. All rights reserved.

#include "token.h"

#include <utility>

Token::Token(std::string value) {
  this->value = std::move(value);
}
bool Token::operator==(const Token &other) const {
  return this->value == other.value;
}