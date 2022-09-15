//
// Created by gabri on 11/9/2022.
//

#include "pattern.h"

#include <utility>

Pattern::Pattern(Entity *stmt, Entity *variable, std::string expr_) {
  this->stmt_ = stmt;
  this->variable_ = variable;
  this->expr_ = std::move(expr_);
}
Entity *Pattern::GetStmt() { return this->stmt_; }
Entity *Pattern::GetVariable() { return this->variable_; }
std::string Pattern::GetExpr() const { return this->expr_; }
std::string Pattern::ToString() {
  return this->stmt_->ToString() + " : " + this->variable_->ToString() + " = " + this->expr_;
  ;
}
