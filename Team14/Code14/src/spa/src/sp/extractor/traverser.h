// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>

#include "sp/node/node.h"

class Traverser {
 private:
 public:
  static void Visit(Node *, const std::function<void(Node *)> &op);
  static void TraverseProgram(ProgramNode *, const std::function<void(Node *)> &op);
  static void TraverseProcedure(ProcedureNode *, const std::function<void(Node *)> &op);
  static void TraverseStmt(StmtNode *, const std::function<void(Node *)> &op);
  static void TraverseAssign(AssignNode *, const std::function<void(Node *)> &op);
  static void TraverseCall(CallNode *, const std::function<void(Node *)> &op);
  static void TraverseIf(IfNode *, const std::function<void(Node *)> &op);
  static void TraverseWhile(WhileNode *, const std::function<void(Node *)> &op);
  static void TraversePrint(PrintNode *, const std::function<void(Node *)> &op);
  static void TraverseRead(ReadNode *, const std::function<void(Node *)> &op);
  static void TraverseCond(CondNode *, const std::function<void(Node *)> &op);
  static void TraverseExpr(ExprNode *, const std::function<void(Node *)> &op);
  static void TraverseVariable(VariableNode *, const std::function<void(Node *)> &op);
  static void TraverseConstant(ConstantNode *, const std::function<void(Node *)> &op);
};
