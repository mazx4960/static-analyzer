// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/parser/node/node.h"
#include "commons/pattern.h"
#include "commons/relationship.h"

class PatternExtractor {
 public:
  static std::vector<Pattern *> ExtractAll(Node *node);
};
