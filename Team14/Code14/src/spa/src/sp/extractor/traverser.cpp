// Copyright 2022 CS3203 Team14. All rights reserved.
#include "traverser.h"

#include "sp/ast/simpleast.h"

void Traverser::TraverseProgram(ProgramNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  for (auto *child : node->GetProcedures()) {
    TraverseProcedure(child, before, after);
  }
  after(node);
}
void Traverser::TraverseProcedure(ProcedureNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  auto *stmt_list = node->GetStatementList();
  for (auto *child : stmt_list->GetStatements()) {
    TraverseStmt(child, before, after);
  }
  after(node);
}
void Traverser::TraverseStmt(StatementNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  switch (node->GetNodeType()) {
    case SimpleNodeType::kAssign:
      TraverseAssign(static_cast<AssignNode *>(node), before, after);
      break;
    case SimpleNodeType::kCall:
      TraverseCall(static_cast<CallNode *>(node), before, after);
      break;
    case SimpleNodeType::kIf:
      TraverseIf(static_cast<IfNode *>(node), before, after);
      break;
    case SimpleNodeType::kWhile:
      TraverseWhile(static_cast<WhileNode *>(node), before, after);
      break;
    case SimpleNodeType::kPrint:
      TraversePrint(static_cast<PrintNode *>(node), before, after);
      break;
    case SimpleNodeType::kRead:
      TraverseRead(static_cast<ReadNode *>(node), before, after);
      break;
    default:
      break;
  }
}
void Traverser::TraverseAssign(AssignNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseCall(CallNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseIf(IfNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseWhile(WhileNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraversePrint(PrintNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseRead(ReadNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseCond(CondExprNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseExpr(ExprNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseVariable(VariableNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}
void Traverser::TraverseConstant(ConstantNode *node, const std::function<void(SimpleAstNode *)> &before, const std::function<void(SimpleAstNode *)> &after) {
  before(node);
  after(node);
}