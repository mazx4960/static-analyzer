// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/token/token.h"

#include <string>
#include <utility>

enum class TokenType {
  SYMBOL,
  LITERAL,
  KEYWORD,
  OPERATOR,
  SEPARATOR,
  END_OF_FILE
};

class SimpleToken : public Token{
 public:
  SimpleToken(TokenType type, std::string value) : Token(std::move(value)) {
    this->type = type;
  };
  TokenType type;
  bool operator==(const SimpleToken &other) const;
};

class SymbolToken : public SimpleToken {
 public:
  explicit SymbolToken(std::string value)
      : SimpleToken(TokenType::SYMBOL, std::move(value)) {
  }
};

class LiteralToken : public SimpleToken {
 public:
  explicit LiteralToken(std::string value)
      : SimpleToken(TokenType::LITERAL, std::move(value)) {
  }
};

class KeywordToken : public SimpleToken {
 public:
  explicit KeywordToken(std::string value)
      : SimpleToken(TokenType::KEYWORD, std::move(value)) {
  }
};

class OperatorToken : public SimpleToken {
 public:
  explicit OperatorToken(std::string value)
      : SimpleToken(TokenType::OPERATOR, std::move(value)) {
  }
};

class SeparatorToken : public SimpleToken {
 public:
  explicit SeparatorToken(std::string value)
      : SimpleToken(TokenType::SEPARATOR, std::move(value)) {
  }
};

class EndOfFileToken : public SimpleToken {
 public:
  EndOfFileToken() : SimpleToken(TokenType::END_OF_FILE, "") {
  }
};
