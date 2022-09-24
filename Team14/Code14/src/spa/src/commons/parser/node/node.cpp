// Copyright 2022 CS3203 Team14. All rights reserved.

#include "node.h"

Node::Node(NodeType nodeType) : nodeType_(nodeType) {}
NodeType Node::GetNodeType() const { return nodeType_; }
/**
 * DFS to visit all nodes
 * prevents infinite loop by keeping track of visited nodes
 * @param op
 */
void Node::VisitAll(const std::function<void(Node *)> &op) {
  std::unordered_map<Node *, bool> visited;
  std::vector<Node *> stack;
  stack.push_back(this);
  while (!stack.empty()) {
    Node *node = stack.back();
    stack.pop_back();
    if (visited[node]) { continue; }
    visited[node] = true;
    op(node);
    for (Node *child : node->GetChildren()) { stack.push_back(child); }
  }
}
/**
 * Visit all children of this node.
 *
 * @param op function to be applied to each child
 */
void Node::VisitChildren(const std::function<void(Node *)> &op) {
  for (auto *child : this->GetChildren()) { op(child); }
}
