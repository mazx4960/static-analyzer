// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

#include "sp/ast/simple_ast.h"

void Traverser::TraverseNode(SimpleAstNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
  for (auto *child : node->GetChildren()) {
    TraverseNode(child, op);
  }
}