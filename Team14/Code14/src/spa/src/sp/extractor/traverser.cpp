// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

void Traverser::Visit(Node *node, const std::function<void(Node *)> &op) {
  op(node);
}
void Traverser::TraverseProgram(ProgramNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
  for (auto child : node->GetProcedures()) {
    TraverseProcedure(child, op);
  }
}
void Traverser::TraverseProcedure(ProcedureNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
  for (auto child : node->GetStmts()) {
    TraverseStmt(child, op);
  }
}
void Traverser::TraverseStmt(StmtNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
  switch (node->GetType()) {
    case NodeType::kAssign:
      TraverseAssign(node, op);
      break;
    case NodeType::kCall:
      TraverseCall(node, op);
      break;
    case NodeType::kIf:
      TraverseIf(node, op);
      break;
    case NodeType::kWhile:
      TraverseWhile(node, op);
      break;
    case NodeType::kPrint:
      TraversePrint(node, op);
      break;
    case NodeType::kRead:
      TraverseRead(node, op);
      break;
    default:
      break;
  }
}
void Traverser::TraverseAssign(AssignNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseCall(CallNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseIf(IfNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseWhile(WhileNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraversePrint(PrintNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseRead(ReadNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseCond(CondNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseExpr(ExprNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseVariable(VariableNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}
void Traverser::TraverseConstant(ConstantNode *node, const std::function<void(Node *)> &op) {
  Visit(node, op);
}