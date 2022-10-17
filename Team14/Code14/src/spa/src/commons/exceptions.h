#pragma once

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error {
 public:
  SyntaxError(const std::string &message) : std::runtime_error(
      "Syntax Error: " + message) {
  }
};

class SemanticError : public std::runtime_error {
 public:
  SemanticError(const std::string &message) : std::runtime_error(
      "SemanticError Error: " + message) {
  }
};