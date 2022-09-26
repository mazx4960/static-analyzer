// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "entity.h"
#include "types.h"

class Relationship {
 private:
  RsType type_;

  Entity *first_;

  Entity *second_;

 public:
  Relationship(RsType type, Entity *first, Entity *second);
  RsType GetType();
  Entity *GetFirst();
  Entity *GetSecond();
  bool operator==(const Relationship &other) const;
  std::string ToString();
};

class FollowsRelationship : public Relationship {
 public:
  FollowsRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kFollows, std::move(first), std::move(second)) {
  }
};

class ParentRelationship : public Relationship {
 public:
  ParentRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kParent, std::move(first), std::move(second)) {
  }
};

class UsesRelationship : public Relationship {
 public:
  UsesRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kUses, std::move(first), std::move(second)) {
  }
};

class ModifiesRelationship : public Relationship {
 public:
  ModifiesRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kModifies, std::move(first), std::move(second)) {
  }
};

class CallsRelationship : public Relationship {
 public:
  CallsRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kCalls, std::move(first), std::move(second)) {
  }
};

class NextRelationship : public Relationship {
 public:
  NextRelationship(Entity *first, Entity *second)
      : Relationship(RsType::kNext, std::move(first), std::move(second)) {
  }
};