//
// Created by gabri on 11/9/2022.
//

#include "pattern.h"

Entity *Pattern::GetStmt() const {
  return this->stmt_;
}
Entity *Pattern::GetVariable() const {
  return this->variable_;
}
std::string Pattern::GetExpr() const {
  return this->expr_;
}
std::string Pattern::ToString() {
  return this->stmt_->ToString() + " : " + this->variable_->ToString() + " = " + this->expr_;
}
