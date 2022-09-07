// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <functional>

#include "commons/parser/node/node.h"

class Traverser {
 private:
 public:
  static void TraverseNode(Node *, const std::function<void(Node *)> &op);
};
