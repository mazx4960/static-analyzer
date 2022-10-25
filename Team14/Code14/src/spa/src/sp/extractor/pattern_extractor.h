// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/parser/node/node.h"
#include "commons/pattern.h"
#include "commons/relationship.h"
#include "entity_extractor.h"
#include "sp/simple_definition/simple_ast.h"

class PatternExtractor {
 public:
  static std::vector<Pattern *> ExtractAll(Node *node);
  static void ExtractAssign(std::vector<Pattern *> &patterns, Node *node);
  static void ExtractIf(std::vector<Pattern *> &patterns, Node *node);
  static void ExtractWhile(std::vector<Pattern *> &patterns, Node *node);
};
