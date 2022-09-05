// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/lexer/token.h"
#include "sp/ast/simple_ast.h"

class SimpleParser {
 private:
  std::vector<Token*> tokens_;

 public:
  explicit SimpleParser(std::vector<Token*> tokens);
  SimpleAstNode* Parse();
};
