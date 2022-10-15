// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>

class BuilderError : public std::runtime_error {
 public:
  explicit BuilderError(const std::string &message) : std::runtime_error("Query build Error: " + message) {
  }
};

class EvaluationStrategyError : public std::runtime_error {
 public:
  explicit EvaluationStrategyError(const std::string &message) : std::runtime_error(message) {
  };
};
