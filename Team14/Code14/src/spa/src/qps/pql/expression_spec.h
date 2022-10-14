// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>
/*
 * Expression used in pattern
 */
class ExpressionSpec {
 private:
  std::string expr_;
 protected:
  explicit ExpressionSpec(std::string expr) : expr_(std::move(expr)) {
  };
 public:
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] virtual bool isWild() const = 0;
  virtual bool operator==(const ExpressionSpec &other) const = 0;
};

class ExactExpression : public ExpressionSpec {
 public:
  explicit ExactExpression(std::string expr) : ExpressionSpec(std::move(expr)) {
  };
  [[nodiscard]] bool isWild() const override;
  bool operator==(const ExpressionSpec &other) const override;
};

class WildExpression : public ExpressionSpec {
 public:
  explicit WildExpression(std::string expr = "") : ExpressionSpec(std::move(expr)) {
  };
  [[nodiscard]] bool isWild() const override;
  bool operator==(const ExpressionSpec &other) const override;
};
