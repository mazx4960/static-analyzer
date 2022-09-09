// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>

#include "qps/pql/query_declaration/query_declaration.h"

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
  explicit QueryClause(ClauseType clauseType) : clause_type_(std::move(clauseType)) {};
  ClauseType clause_type_;

 public:
  ClauseType getClauseType();
};

class SuchThatClause : public QueryClause {
 public:
  explicit SuchThatClause(SuchThatType type, QueryDeclaration* first, QueryDeclaration* second)
  : QueryClause(ClauseType::kSuchThat), type_(type), first_(std::move(first)), second_(std::move(second)) {};
  SuchThatType type_;
  QueryDeclaration* first_;
  QueryDeclaration* second_;
  SuchThatType getType();
  QueryDeclaration* getFirst();
  QueryDeclaration* getSecond();

};

class PatternClause : public QueryClause {
 public:
  explicit PatternClause(PatternType type, QueryDeclaration* first, QueryDeclaration* second, QueryDeclaration* third)
      : QueryClause(ClauseType::kSuchThat),
      type_(type), first_(std::move(first)), second_(std::move(second)), third_(std::move(third)) {};
  PatternType type_;
  QueryDeclaration* first_;
  QueryDeclaration* second_;
  QueryDeclaration* third_;
  PatternType getType();
  QueryDeclaration* getFirst();
  QueryDeclaration* getSecond();
  QueryDeclaration* getThird();

};

class ParentClause : public SuchThatClause {
 public:
  explicit ParentClause(QueryDeclaration* first, QueryDeclaration* second)
  : SuchThatClause(SuchThatType::kParent, std::move(first), std::move(second)){}
};

class ParentAllClause : public SuchThatClause {
 public:
  explicit ParentAllClause(QueryDeclaration* first, QueryDeclaration* second)
      : SuchThatClause(SuchThatType::kParentAll, std::move(first), std::move(second)){}
};

class FollowsClause : public SuchThatClause {
 public:
  explicit FollowsClause(QueryDeclaration* first, QueryDeclaration* second)
      : SuchThatClause(SuchThatType::kFollows, std::move(first), std::move(second)){}
};

class FollowsAllClause : public SuchThatClause {
 public:
  explicit FollowsAllClause(QueryDeclaration* first, QueryDeclaration* second)
      : SuchThatClause(SuchThatType::kFollowsAll, std::move(first), std::move(second)){}
};

class UsesClause : public SuchThatClause {
 public:
  explicit UsesClause(QueryDeclaration* first, QueryDeclaration* second)
      : SuchThatClause(SuchThatType::kUses, std::move(first), std::move(second)){}
};

class ModifiesClause : public SuchThatClause {
 public:
  explicit ModifiesClause(QueryDeclaration* first, QueryDeclaration* second)
      : SuchThatClause(SuchThatType::kModifies, std::move(first), std::move(second)){}
};

class AssignPatternClause : public PatternClause {
 public:
  explicit AssignPatternClause(QueryDeclaration* first, QueryDeclaration* second, QueryDeclaration* third)
      : PatternClause(PatternType::kAssignPattern, std::move(first), std::move(second), std::move(third)){}
};
