// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>

#include "sp/node/node.h"

class Traverser {
 private:
 public:
  Traverser() = default;
  virtual ~Traverser() = default;
  static void Visit(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseProgram(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseProcedure(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseStmt(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseAssign(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseCall(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseIf(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseWhile(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraversePrint(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseRead(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseCond(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
  static void TraverseExpr(Node *, const std::function<bool(Node *)> &test, const std::function<void(Node *)> &op);
};
