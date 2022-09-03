// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

enum class TokenType {
  SYMBOL,
  LITERAL,
  OPERATOR,
  SEPARATOR,
  END_OF_FILE
};

class Token {
 public:
  Token(TokenType type, std::string value);
  TokenType type;
  std::string value;
  bool operator==(const Token &other) const;
};

class SymbolToken : public Token {
 public:
  explicit SymbolToken(std::string value)
      : Token(TokenType::SYMBOL, std::move(value)) {}
  const bool isKeyword = false;
};

class KeywordToken : public SymbolToken {
 public:
  explicit KeywordToken(std::string value)
      : SymbolToken(std::move(value)) {}
  const bool isKeyword = true;
};

class LiteralToken : public Token {
 public:
  explicit LiteralToken(std::string value)
      : Token(TokenType::LITERAL, std::move(value)) {}
};

class OperatorToken : public Token {
 public:
  explicit OperatorToken(std::string value)
      : Token(TokenType::OPERATOR, std::move(value)) {}
};

class SeparatorToken : public Token {
 public:
  explicit SeparatorToken(std::string value)
      : Token(TokenType::SEPARATOR, std::move(value)) {}
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken() : Token(TokenType::END_OF_FILE, "") {}
};
