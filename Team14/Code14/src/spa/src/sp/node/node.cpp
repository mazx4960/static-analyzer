// Copyright 2022 CS3203 Team14. All rights reserved.
#include "node.h"

Node::Node() = default;
Node::~Node() = default;
NodeType Node::GetType() {
  return this->type_;
}
