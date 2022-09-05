// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>

#include "sp/ast/simple_ast.h"

class Traverser {
 private:
 public:
  static void TraverseNode(SimpleAstNode *, const std::function<void(SimpleAstNode *)> &op);
};
