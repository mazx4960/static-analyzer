//
// Created by gabri on 11/9/2022.
//

#pragma once
#include <utility>

#include "commons/parser/expr_definition/expr_node.h"
#include "entity.h"

class Pattern {
 private:
  Entity *stmt_;

  Entity *variable_;

  std::string expr_;

 public:
  Pattern(Entity *stmt, Entity *variable, std::string expr_) : stmt_(stmt), variable_(variable), expr_(std::move(expr_)) {
  };
  [[nodiscard]] Entity *GetStmt() const;
  [[nodiscard]] Entity *GetVariable() const;
  [[nodiscard]] std::string GetExpr() const;
  std::string ToString();
};
