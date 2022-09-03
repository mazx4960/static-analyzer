// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "entity.h"

enum class RsType {
  Follows,
  Parent,
  Uses,
  Modifies,
};

class Relationship {
 private:
  RsType type;
  Entity first;
  Entity second;

 public:
  Relationship(RsType type, Entity first, Entity second);
  bool operator==(const Relationship &other) const;
};

class FollowsRelationship : public Relationship {
 public:
  FollowsRelationship(Entity first, Entity second)
      : Relationship(RsType::Follows, std::move(first), std::move(second)) {}
};

class ParentRelationship : public Relationship {
 public:
  ParentRelationship(Entity first, Entity second)
      : Relationship(RsType::Parent, std::move(first), std::move(second)) {}
};

class UsesRelationship : public Relationship {
 public:
  UsesRelationship(Entity first, Entity second)
      : Relationship(RsType::Uses, std::move(first), std::move(second)) {}
};

class ModifiesRelationship : public Relationship {
 public:
  ModifiesRelationship(Entity first, Entity second)
      : Relationship(RsType::Modifies, std::move(first), std::move(second)) {}
};
