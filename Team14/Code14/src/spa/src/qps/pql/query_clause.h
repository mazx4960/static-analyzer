// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "query_declaration.h"

enum class ClauseType {
  kSuchThat,
  kPattern,
};

enum class SuchThatType {
  kParent,
  kParentAll,
  kFollows,
  kFollowsAll,
  kUses,
  kModifies,
};

enum class PatternType {
  kAssignPattern,
};

class QueryClause {
 protected:
  explicit QueryClause(ClauseType clauseType) : clause_type_(clauseType) {};

  ClauseType clause_type_;

 public:
  ClauseType getClauseType();
};

class SuchThatClause : public QueryClause {
 public:
  explicit SuchThatClause(SuchThatType type, QueryDeclaration *first, QueryDeclaration *second)
      : QueryClause(ClauseType::kSuchThat), type_(type), first_(first), second_(second) {};

  SuchThatType type_;

  QueryDeclaration *first_;

  QueryDeclaration *second_;

  [[nodiscard]] SuchThatType getSuchThatType() const;
  [[nodiscard]] QueryDeclaration *getFirst() const;
  [[nodiscard]] QueryDeclaration *getSecond() const;
};

class PatternClause : public QueryClause {
 public:
  explicit PatternClause(PatternType type, QueryDeclaration *first, QueryDeclaration *second, QueryDeclaration *third)
      : QueryClause(ClauseType::kSuchThat), type_(type), first_(first), second_(second), third_(third) {};

  PatternType type_;

  QueryDeclaration *first_;

  QueryDeclaration *second_;

  QueryDeclaration *third_;

  [[nodiscard]] PatternType getPatternType() const;
  [[nodiscard]] QueryDeclaration *getFirst() const;
  [[nodiscard]] QueryDeclaration *getSecond() const;
  [[nodiscard]] QueryDeclaration *getThird() const;
};

class ParentClause : public SuchThatClause {
 public:
  explicit ParentClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kParent, first, second) {}
};

class ParentAllClause : public SuchThatClause {
 public:
  explicit ParentAllClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kParentAll, first, second) {}
};

class FollowsClause : public SuchThatClause {
 public:
  explicit FollowsClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kFollows, first, second) {}
};

class FollowsAllClause : public SuchThatClause {
 public:
  explicit FollowsAllClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kFollowsAll, first, second) {}
};

class UsesClause : public SuchThatClause {
 public:
  explicit UsesClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kUses, first, second) {}
};

class ModifiesClause : public SuchThatClause {
 public:
  explicit ModifiesClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(SuchThatType::kModifies, first, second) {}
};

class AssignPatternClause : public PatternClause {
 public:
  explicit AssignPatternClause(QueryDeclaration *first, QueryDeclaration *second, QueryDeclaration *third)
      : PatternClause(PatternType::kAssignPattern, first, second, third) {}
};

class QueryClauseTypeAdapter {};