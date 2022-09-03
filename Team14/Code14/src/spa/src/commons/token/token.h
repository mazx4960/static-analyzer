// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

enum class TokenType {
  kSymbol,
  kLiteral,
  kOperator,
  kQuote,
  kComma,
  kSemicolon,
  kRoundOpenBracket,
  kRoundCloseBracket,
  kCurlyOpenBracket,
  kCurlyCloseBracket,
  kEndOfFile
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
      : Token(TokenType::kSymbol, std::move(value)) {}
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
      : Token(TokenType::kLiteral, std::move(value)) {}
};

class OperatorToken : public Token {
 public:
  explicit OperatorToken(std::string value)
      : Token(TokenType::kOperator, std::move(value)) {}
};

class QuoteToken : public Token {
 public:
  explicit QuoteToken(std::string value)
      : Token(TokenType::kQuote, std::move(value)) {}
};

class CommaToken : public Token {
 public:
  explicit CommaToken(std::string value)
      : Token(TokenType::kComma, std::move(value)) {}
};

class SemicolonToken : public Token {
 public:
  explicit SemicolonToken(std::string value)
      : Token(TokenType::kSemicolon, std::move(value)) {}
};

class RoundOpenBracketToken : public Token {
 public:
  explicit RoundOpenBracketToken(std::string value)
      : Token(TokenType::kRoundOpenBracket, std::move(value)) {}
};

class RoundCloseBracketToken : public Token {
 public:
  explicit RoundCloseBracketToken(std::string value)
      : Token(TokenType::kRoundCloseBracket, std::move(value)) {}
};

class CurlyOpenBracketToken : public Token {
 public:
  explicit CurlyOpenBracketToken(std::string value)
      : Token(TokenType::kCurlyOpenBracket, std::move(value)) {}
};

class CurlyCloseBracket : public Token {
 public:
  explicit CurlyCloseBracket(std::string value)
      : Token(TokenType::kCurlyCloseBracket, std::move(value)) {}
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken() : Token(TokenType::kEndOfFile, "") {}
};
