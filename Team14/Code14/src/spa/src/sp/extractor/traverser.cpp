// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

#include "sp/ast/simpleast.h"

void Traverser::TraverseProgram(ProgramNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
  for (auto *child : node->GetProcedures()) {
    TraverseProcedure(child, op);
  }
}
void Traverser::TraverseProcedure(ProcedureNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
  auto *stmt_list = node->GetStatementList();
  for (auto *child : stmt_list->GetStatements()) {
    TraverseStmt(child, op);
  }
}
void Traverser::TraverseStmt(StatementNode *node, const std::function<void(SimpleAstNode *)> &op) {
  switch (node->GetNodeType()) {
    case SimpleNodeType::kAssign:
      TraverseAssign(static_cast<AssignNode *>(node), op);
      break;
    case SimpleNodeType::kCall:
      TraverseCall(static_cast<CallNode *>(node), op);
      break;
    case SimpleNodeType::kIf:
      TraverseIf(static_cast<IfNode *>(node), op);
      break;
    case SimpleNodeType::kWhile:
      TraverseWhile(static_cast<WhileNode *>(node), op);
      break;
    case SimpleNodeType::kPrint:
      TraversePrint(static_cast<PrintNode *>(node), op);
      break;
    case SimpleNodeType::kRead:
      TraverseRead(static_cast<ReadNode *>(node), op);
      break;
    default:
      break;
  }
}
void Traverser::TraverseAssign(AssignNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseCall(CallNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseIf(IfNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseWhile(WhileNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraversePrint(PrintNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseRead(ReadNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseCond(CondExprNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseExpr(ExprNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseVariable(VariableNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}
void Traverser::TraverseConstant(ConstantNode *node, const std::function<void(SimpleAstNode *)> &op) {
  op(node);
}