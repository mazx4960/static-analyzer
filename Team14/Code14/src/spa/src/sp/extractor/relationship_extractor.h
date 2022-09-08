// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <vector>

#include "commons/parser/node/node.h"
#include "commons/relationship.h"

class RelationshipExtractor {
 public:
  static std::vector<Relationship *> ExtractAll(Node *node);
  static void Match(std::vector<Relationship *> &relationships, RsType rs_type, Entity *parent,
                    const std::vector<Entity *> &children);
  static void ExtractFollows(std::vector<Relationship *> &relationships, Node *node);
  static void ExtractParent(std::vector<Relationship *> &relationships, Node *node);
  static void ExtractParentHelper(std::vector<Relationship *> &relationships, Entity *parent, Node *node);
  static void ExtractUses(std::vector<Relationship *> &relationships, Node *node);
  static void ExtractUsesRecursive(std::vector<Relationship *> &relationships, Entity *parent, Node *node);
  static void ExtractModifies(std::vector<Relationship *> &relationships, Node *node);
  static void ExtractModifiesRecursive(std::vector<Relationship *> &relationships, Entity *parent, Node *node);
};
