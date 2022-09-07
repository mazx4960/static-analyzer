// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

void Traverser::TraverseNode(Node *node, const std::function<void(Node *)> &op) {
  op(node);
  for (auto *child : node->GetChildren()) { TraverseNode(child, op); }
}