// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity_extractor.h"

#include "sp/ast/simpleast.h"
#include "traverser.h"

std::vector<Entity> EntityExtractor::extract() {
  auto before = [](SimpleAstNode *node) {};
  auto after = [](SimpleAstNode *node) {};
  Traverser::TraverseProgram(program_node_, before, after);
  return std::vector<Entity>();
}
