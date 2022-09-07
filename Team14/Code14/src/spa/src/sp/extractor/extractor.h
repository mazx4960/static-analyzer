// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "commons/entity.h"
#include "commons/parser/node/node.h"
#include "commons/relationship.h"

class EntityExtractor {
 public:
  static std::vector<Entity *> Extract(Node *program_node);
};

class RelationshipExtractor {
 public:
  static std::vector<Relationship *> Extract(Node *program_node);
  static void ExtractFollows(Node *node, std::vector<Relationship *> &relationships);
  static void ExtractParent(Node *node, std::vector<Relationship *> &relationships);
  static void ExtractParentHelper(Entity *parent, Node *node, std::vector<Relationship *> &relationships);
  static std::vector<Entity *> ExtractChildren(Node *node);
};