#pragma once

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error {
 public:
  explicit SyntaxError(const std::string &message) : std::runtime_error(
      "Syntax Error: " + message) {
  }
};

class SemanticError : public std::runtime_error {
 public:
  explicit SemanticError(const std::string &message) : std::runtime_error(
      "SemanticError Error: " + message) {
  }
};