// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/relationship.h"
#include "sp/ast/simple_ast.h"

class EntityExtractor {
 public:
  static std::vector<Entity *> Extract(SimpleAstNode *program_node);
};

class RelationshipExtractor {
 public:
  static std::vector<Relationship *> Extract(SimpleAstNode *program_node);
  static void ExtractFollows(SimpleAstNode *node, std::vector<Relationship *> &relationships);
  static void ExtractParent(SimpleAstNode *node, std::vector<Relationship *> &relationships);
  static void ExtractParentHelper(Entity *parent, StatementListNode *node, std::vector<Relationship *> &relationships);
  static std::vector<Entity *> ExtractChildren(StatementListNode *node);
};