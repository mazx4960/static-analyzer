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
  kWildCard,
  kEndOfFile
};

class Token {
 public:
  Token(TokenType type, std::string value);

  TokenType type;

  std::string value;

  bool operator==(const Token &other) const;
  bool operator!=(const Token &other) const;
  [[nodiscard]] std::string ToString() const;
};

class SymbolToken : public Token {
 public:
  explicit SymbolToken(std::string value) : Token(TokenType::kSymbol, std::move(value)) {
  }

  const bool isKeyword = false;
};

class KeywordToken : public SymbolToken {
 public:
  explicit KeywordToken(std::string value) : SymbolToken(std::move(value)) {
  }

  const bool isKeyword = true;
};

class LiteralToken : public Token {
 public:
  explicit LiteralToken(std::string value) : Token(TokenType::kLiteral, std::move(value)) {
  }
};

class OperatorToken : public Token {
 public:
  explicit OperatorToken(std::string value) : Token(TokenType::kOperator, std::move(value)) {
  }
};

class QuoteToken : public Token {
 public:
  explicit QuoteToken() : Token(TokenType::kQuote, "\"") {
  }
};

class CommaToken : public Token {
 public:
  explicit CommaToken() : Token(TokenType::kComma, ",") {
  }
};

class SemicolonToken : public Token {
 public:
  explicit SemicolonToken() : Token(TokenType::kSemicolon, ";") {
  }
};

class RoundOpenBracketToken : public Token {
 public:
  explicit RoundOpenBracketToken() : Token(TokenType::kRoundOpenBracket, "(") {
  }
};

class RoundCloseBracketToken : public Token {
 public:
  explicit RoundCloseBracketToken() : Token(TokenType::kRoundCloseBracket, ")") {
  }
};

class CurlyOpenBracketToken : public Token {
 public:
  explicit CurlyOpenBracketToken() : Token(TokenType::kCurlyOpenBracket, "{") {
  }
};

class CurlyCloseBracketToken : public Token {
 public:
  explicit CurlyCloseBracketToken()
      : Token(TokenType::kCurlyCloseBracket, "}") {
  }
};

class WildCardToken : public Token {
 public:
  explicit WildCardToken()
      : Token(TokenType::kWildCard, "_") {
  }
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken() : Token(TokenType::kEndOfFile, "") {
  }
};
