// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

void Traverser::Visit(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  if (test(node)) {
    op(node);
  }
}
void Traverser::TraverseProgram(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
  for (auto child : node->GetChildren()) {
    TraverseProcedure(child, test, op);
  }
}
void Traverser::TraverseProcedure(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
  for (auto child : node->GetChildren()) {
    TraverseStmt(child, test, op);
  }
}
void Traverser::TraverseStmt(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
  switch (node->GetType()) {
    case NodeType::kAssign:
      TraverseAssign(node, test, op);
      break;
    case NodeType::kCall:
      TraverseCall(node, test, op);
      break;
    case NodeType::kIf:
      TraverseIf(node, test, op);
      break;
    case NodeType::kWhile:
      TraverseWhile(node, test, op);
      break;
    case NodeType::kPrint:
      TraversePrint(node, test, op);
      break;
    case NodeType::kRead:
      TraverseRead(node, test, op);
      break;
    default:
      break;
  }
}
void Traverser::TraverseAssign(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseCall(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseIf(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseWhile(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraversePrint(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseRead(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseCond(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
void Traverser::TraverseExpr(Node *node, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op) {
  Visit(node, test, op);
}
