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
 private:
  RsType type_;

  QueryDeclaration *first_;

  QueryDeclaration *second_;
 public:
  explicit SuchThatClause(RsType type, QueryDeclaration *first, QueryDeclaration *second)
      : QueryClause(ClauseType::kSuchThat), type_(type), first_(first), second_(second) {
  };
  [[nodiscard]] RsType getSuchThatType() const;
  [[nodiscard]] QueryDeclaration *getFirst() const;
  [[nodiscard]] QueryDeclaration *getSecond() const;
  void setFirst(SynonymDeclaration *synonym_declaration);
  void setSecond(SynonymDeclaration *synonym_declaration);
};

class PatternClause : public QueryClause {
 private:
  RsType type_;

  SynonymDeclaration *synonym_declaration_;

  QueryDeclaration *ent_ref_;

  StaticDeclaration *expression_;
 public:
  explicit PatternClause(RsType type, SynonymDeclaration *synonym_declaration, QueryDeclaration *ent_ref, StaticDeclaration *expression)
      : QueryClause(ClauseType::kPattern), type_(type), synonym_declaration_(synonym_declaration), ent_ref_(ent_ref), expression_(expression) {
  };

  [[nodiscard]] RsType getPatternType() const;
  [[nodiscard]] SynonymDeclaration *getSynonymDeclaration() const;
  void setSynonymDeclaration(SynonymDeclaration *synonym_declaration);
  [[nodiscard]] QueryDeclaration *getEntRef() const;
  [[nodiscard]] StaticDeclaration *getExpression() const;
};

class AssignPatternClause : public PatternClause {
 public:
  explicit AssignPatternClause(SynonymDeclaration *synonym_declaration, QueryDeclaration *ent_ref, StaticDeclaration *expression)
      : PatternClause(RsType::kAssignPattern, synonym_declaration, ent_ref, expression) {
  }
};
