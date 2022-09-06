#pragma once

#include <stdexcept>
#include <string>

class ParseSyntaxError : public std::runtime_error {
 public:
  explicit ParseSyntaxError(const std::string& message) : std::runtime_error(
      "Parse Syntax Error: " + message) {}
};