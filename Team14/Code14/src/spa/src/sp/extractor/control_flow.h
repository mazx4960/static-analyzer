// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/parser/node/node.h"
#include "commons/types.h"

class CFGNode {
 private:
  Entity *stmt_;
  std::vector<CFGNode *> children_;

 public:
  explicit CFGNode(Entity *stmt);
  Entity *GetStmt();
  void AddChild(CFGNode *child);
  void SetChildren(std::vector<CFGNode *> children);
  bool IsValid();
  std::vector<CFGNode *> GetChildren();
  std::string ToString();
  void VisitAll(const std::function<void(CFGNode *)> &op);
};

class CFGBuilder {
 private:
  CFGNode *start_node_;
  static CFGNode *ToCFGNode(Node *node);
  static void ConnectNode(CFGNode *parent, CFGNode *child);
  static CFGNode *BuildBlock(Node *node, CFGNode *parent, CFGNode *terminal);
  static CFGNode *BuildIf(Node *node, CFGNode *parent);
  static CFGNode *BuildWhile(Node *node, CFGNode *parent);

 public:
  explicit CFGBuilder();
  CFGNode *Build(Node *node);
  void Clean();
};
