// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>
#include <vector>

#include "node_type.h"

class Node {
 private:
  NodeType nodeType_;

 public:
  explicit Node(NodeType nodeType);
  [[nodiscard]] NodeType GetNodeType() const;
  virtual std::vector<Node *> GetChildren() = 0;
  void VisitAll(const std::function<void(Node *)> &op);
  void VisitChildren(const std::function<void(Node *)> &op);
};
