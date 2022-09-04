// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>

#include "sp/ast/simpleast.h"

class Traverser {
 private:
 public:
  static void TraverseProgram(ProgramNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseProcedure(ProcedureNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseStmt(StatementNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseAssign(AssignNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseCall(CallNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseIf(IfNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseWhile(WhileNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraversePrint(PrintNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseRead(ReadNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseCond(CondExprNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseExpr(ExprNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseVariable(VariableNode *, const std::function<void(SimpleAstNode *)> &op);
  static void TraverseConstant(ConstantNode *, const std::function<void(SimpleAstNode *)> &op);
};
