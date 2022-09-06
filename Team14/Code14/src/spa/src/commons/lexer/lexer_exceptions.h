#pragma once

#include <stdexcept>
#include <string>

class LexSyntaxError : public std::runtime_error {
 public:
  LexSyntaxError(int line_number, int column_number, const std::string &message) : std::runtime_error(
      "Syntax Error: " + std::to_string(line_number) + ":" + std::to_string(column_number) + " " + message) {}
};