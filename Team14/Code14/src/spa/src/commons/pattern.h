//
// Created by gabri on 11/9/2022.
//
#pragma once
#include "commons/parser/expr_definition/expr_node.h"
#include "entity.h"
class Pattern {
 private:
  Entity *stmt_;
  Entity *variable_;
  std::string expr_;

 public:
  Pattern(Entity *stmt, Entity *variable, std::string expr_);
  [[nodiscard]] Entity *GetStmt() const;
  Entity *GetVariable();
  [[nodiscard]] std::string GetExpr() const;
  std::string ToString();
};

struct StmtExprHashFunction {
  size_t operator()(const std::pair<Entity *, std::string> &p) const {
    size_t hash = 0;
    HashCombine<Entity *>::hash_combine(hash, p.first);
    HashCombine<std::string>::hash_combine(hash, p.second);
    return hash;
  }
};
