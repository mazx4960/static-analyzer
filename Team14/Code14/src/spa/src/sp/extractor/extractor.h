// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/relationship.h"
#include "sp/ast/simpleast.h"

class Extractor {
 public:
  virtual std::vector<Entity *> ExtractEntity(ProgramNode *program_node){};
  virtual std::vector<Relationship *> ExtractRelationship(ProgramNode *program_node){};
};

class EntityExtractor : public Extractor {
 public:
  std::vector<Entity *> ExtractEntity(ProgramNode *program_node) override;
};

class RelationshipExtractor : public Extractor {
 public:
  std::vector<Relationship *> ExtractRelationship(ProgramNode *program_node) override;
  static std::vector<Relationship *> ExtractFollows(StatementListNode *node);
  static std::vector<Relationship *> ExtractParent(SimpleAstNode *node);
  static std::vector<Relationship *> ExtractParentHelper(Entity *parent, StatementListNode *node);
  static std::vector<Entity *> ExtractChildren(StatementListNode *node);
};