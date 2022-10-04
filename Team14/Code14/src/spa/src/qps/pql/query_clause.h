// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "query_declaration.h"

enum class ClauseType {
  kSuchThat,
  kPattern,
};

inline std::string ClauseTypeToString(ClauseType type) {
  switch (type) {
    case ClauseType::kSuchThat:return "SuchThat-clause";
    case ClauseType::kPattern:return "Pattern-clause";
    default:return "unknown clause type";
  }
}

class QueryClause {
 protected:
  explicit QueryClause(ClauseType clauseType) : clause_type_(clauseType) {
  };

  ClauseType clause_type_;

 public:
  ClauseType getClauseType();
};

class SuchThatClause : public QueryClause {
 public:
  explicit SuchThatClause(RsType type, QueryDeclaration *first, QueryDeclaration *second)
      : QueryClause(ClauseType::kSuchThat), type_(type), first_(first), second_(second) {
  };

  RsType type_;

  QueryDeclaration *first_;

  QueryDeclaration *second_;

  [[nodiscard]] RsType getSuchThatType() const;
  [[nodiscard]] QueryDeclaration *getFirst() const;
  [[nodiscard]] QueryDeclaration *getSecond() const;
};

class PatternClause : public QueryClause {
 public:
  explicit PatternClause(RsType type, QueryDeclaration *first, QueryDeclaration *second, QueryDeclaration *third)
      : QueryClause(ClauseType::kPattern), type_(type), first_(first), second_(second), third_(third) {
  };

  RsType type_;

  QueryDeclaration *first_;

  QueryDeclaration *second_;

  QueryDeclaration *third_;

  [[nodiscard]] RsType getPatternType() const;
  [[nodiscard]] QueryDeclaration *getFirst() const;
  [[nodiscard]] QueryDeclaration *getSecond() const;
  [[nodiscard]] QueryDeclaration *getThird() const;
};

class ParentClause : public SuchThatClause {
 public:
  explicit ParentClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kParent, first, second) {
  }
};

class ParentAllClause : public SuchThatClause {
 public:
  explicit ParentAllClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kParentT, first, second) {
  }
};

class FollowsClause : public SuchThatClause {
 public:
  explicit FollowsClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kFollows, first, second) {
  }
};

class FollowsAllClause : public SuchThatClause {
 public:
  explicit FollowsAllClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kFollowsT, first, second) {
  }
};

class UsesClause : public SuchThatClause {
 public:
  explicit UsesClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kUses, first, second) {
  }
};

class ModifiesClause : public SuchThatClause {
 public:
  explicit ModifiesClause(QueryDeclaration *first, QueryDeclaration *second)
      : SuchThatClause(RsType::kModifies, first, second) {
  }
};

class AssignPatternClause : public PatternClause {
 public:
  explicit AssignPatternClause(QueryDeclaration *first, QueryDeclaration *second, QueryDeclaration *third)
      : PatternClause(RsType::kAssignPattern, first, second, third) {
  }
};
