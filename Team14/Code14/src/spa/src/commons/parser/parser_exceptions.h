#pragma once

#include <stdexcept>
#include <string>
#include "commons/exceptions.h"

class ParseSyntaxError : public SyntaxError {
 public:
  explicit ParseSyntaxError(const std::string &message) : SyntaxError(
      "Parse Syntax Error: " + message) {
  }
};

class ParseSemanticError : public SemanticError{
 public:
  explicit ParseSemanticError(const std::string &message) : SemanticError(
      "Parse Semantic Error: " + message) {
  }
};