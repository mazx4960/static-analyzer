// Copyright 2022 CS3203 Team14. All rights reserved.

#include "node.h"

Node::Node(NodeType nodeType) : nodeType_(nodeType) {}
NodeType Node::GetNodeType() const { return nodeType_; }
void Node::VisitAll(const std::function<void(Node *)> &op) {
  op(this);
  for (auto *child : this->GetChildren()) { child->VisitAll(op); }
}
void Node::VisitChildren(const std::function<void(Node *)> &op) {
  for (auto *child : this->GetChildren()) { op(child); }
}
