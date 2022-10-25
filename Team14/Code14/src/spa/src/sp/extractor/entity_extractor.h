// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/parser/node/node.h"
#include "commons/relationship.h"
#include "sp/simple_definition/simple_ast.h"

class EntityExtractor {
 public:
  static std::vector<Entity *> ExtractAll(Node *node);
  static std::vector<Entity *> ExtractAllVariables(Node *node);
  static std::vector<Entity *> ExtractProcedureCalls(Node *node);
  static void ExtractProcedure(std::vector<Entity *> &entities, Node *node);
  static void ExtractStatement(std::vector<Entity *> &entities, Node *node);
  static Entity *GetStmtEntity(Node *node);
  static void ExtractVariable(std::vector<Entity *> &entities, Node *node);
  static void ExtractConstant(std::vector<Entity *> &entities, Node *node);
};