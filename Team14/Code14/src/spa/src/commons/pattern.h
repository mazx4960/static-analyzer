//
// Created by gabri on 11/9/2022.
//
#pragma once
#include "commons/parser/expr_definition/expr_node.h"
class Pattern {
 private:
  std::string var_name_;
  std::string expr_;
 public:
  Pattern(std::string var_name, std::string expr_);
};
