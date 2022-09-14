//
// Created by gabri on 11/9/2022.
//

#include "pattern.h"

#include <utility>

Pattern::Pattern(std::string var_name, int stmt_no, std::string expr)
    : var_name_(std::move(var_name)),
      stmt_no_(stmt_no),
      expr_(std::move(expr)) {}
std::string Pattern::GetVarName() const { return this->var_name_; }
int Pattern::GetStmtNo() const { return this->stmt_no_; }
std::string Pattern::GetExpr() const { return this->expr_; }
std::string Pattern::ToString() {
  return std::to_string(this->stmt_no_) + ": " + this->var_name_ + " = " + this->expr_;
  ;
}
