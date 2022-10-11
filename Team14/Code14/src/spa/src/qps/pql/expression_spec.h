// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

class ExpressionSpec {
 private:
  std::string expr_;
 protected:
  explicit ExpressionSpec(std::string expr) : expr_(std::move(expr))  {
  };
 public:
  std::string toString();
  virtual bool isWild() = 0;
};

class ExactExpression : public ExpressionSpec {
 public:
  explicit ExactExpression(std::string expr) : ExpressionSpec(std::move(expr)) {
  };
  bool isWild() override;
};

class WildExpression : public ExpressionSpec {
 public:
  explicit WildExpression(std::string expr = "") : ExpressionSpec(std::move(expr)) {
  };
  bool isWild() override;
};
