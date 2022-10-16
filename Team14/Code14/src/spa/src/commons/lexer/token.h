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
  kDot,
  kRoundOpenBracket,
  kRoundCloseBracket,
  kCurlyOpenBracket,
  kCurlyCloseBracket,
  kAngleOpenBracket,
  kAngleCloseBracket,
  kWildCard,
  kHashtag,
  kComparator,
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
};

class KeywordToken : public SymbolToken {
 public:
  explicit KeywordToken(std::string value) : SymbolToken(std::move(value)) {
  }
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

class DotToken : public Token {
 public:
  explicit DotToken() : Token(TokenType::kDot, ".") {
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
  explicit CurlyCloseBracketToken() : Token(TokenType::kCurlyCloseBracket, "}") {
  }
};

class AngleOpenBracketToken : public Token {
 public:
  explicit AngleOpenBracketToken() : Token(TokenType::kAngleOpenBracket, "<") {
  }
};

class AngleCloseBracketToken : public Token {
 public:
  explicit AngleCloseBracketToken() : Token(TokenType::kAngleCloseBracket, ">") {
  }
};

class WildCardToken : public Token {
 public:
  explicit WildCardToken() : Token(TokenType::kWildCard, "_") {
  }
};

class HashtagToken : public Token {
 public:
  explicit HashtagToken() : Token(TokenType::kHashtag, "#") {
  }
};

class ComparatorToken : public Token {
 public:
  explicit ComparatorToken(std::string value) : Token(TokenType::kComparator, std::move(value)) {
  }
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken() : Token(TokenType::kEndOfFile, "") {
  }
};
