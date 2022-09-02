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

class QueryToken : public Token{
 public:
  QueryToken(TokenType type, std::string value) : Token(std::move(value)) {
    this->type = type;
  };
  TokenType type;
  bool operator==(const QueryToken &other) const;
};

class SymbolToken : public QueryToken {
 public:
  explicit SymbolToken(std::string value)
      : QueryToken(TokenType::SYMBOL, std::move(value)) {
  }
};

class LiteralToken : public QueryToken {
 public:
  explicit LiteralToken(std::string value)
      : QueryToken(TokenType::LITERAL, std::move(value)) {
  }
};

class KeywordToken : public QueryToken {
 public:
  explicit KeywordToken(std::string value)
      : QueryToken(TokenType::KEYWORD, std::move(value)) {
  }
};

class OperatorToken : public QueryToken {
 public:
  explicit OperatorToken(std::string value)
      : QueryToken(TokenType::OPERATOR, std::move(value)) {
  }
};

class SeparatorToken : public QueryToken {
 public:
  explicit SeparatorToken(std::string value)
      : QueryToken(TokenType::SEPARATOR, std::move(value)) {
  }
};

class EndOfFileToken : public QueryToken {
 public:
  EndOfFileToken() : QueryToken(TokenType::END_OF_FILE, "") {
  }
};
