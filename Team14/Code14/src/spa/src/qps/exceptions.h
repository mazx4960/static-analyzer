// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>
#include "commons/exceptions.h"

class BuilderError : public SemanticError {
 public:
  explicit BuilderError(const std::string &message) : SemanticError("Query build Error: " + message) {
  }
};

class EvaluationStrategyError : public SemanticError {
 public:
  explicit EvaluationStrategyError(const std::string &message) : SemanticError(message) {
  };
};

class ResultCreationError : public std::runtime_error {
 public:
  explicit ResultCreationError(const std::string &message) : std::runtime_error(message) {
  }
};
