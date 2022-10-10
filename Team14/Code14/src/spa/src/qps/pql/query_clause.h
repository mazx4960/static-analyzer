// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "query_reference.h"
#include "expression_spec.h"
#include "qps/pql/interface/check_semantics.h"

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

class QueryClause : public ICheckSyntax, public ICheckSemantics {
 private:
  ClauseType clause_type_;

 protected:
  explicit QueryClause(ClauseType clauseType) : clause_type_(clauseType) {
  };

 public:
  ClauseType getClauseType();
  [[nodiscard]]bool isSyntacticallyCorrect() const override = 0;
  [[nodiscard]] bool IsSemanticallyCorrect() const override = 0;
};

class SuchThatClause : public QueryClause {
 private:
  RsType type_;

  QueryReference *first_;

  QueryReference *second_;

 protected:
  explicit SuchThatClause(RsType type, QueryReference *first, QueryReference *second)
      : QueryClause(ClauseType::kSuchThat), type_(type), first_(first), second_(second) {
  };

 public:
  [[nodiscard]] RsType getSuchThatType() const;
  [[nodiscard]] QueryReference *getFirst() const;
  [[nodiscard]] QueryReference *getSecond() const;
  void setFirst(SynonymReference *synonym_declaration);
  void setSecond(SynonymReference *synonym_declaration);
  [[nodiscard]] bool isSyntacticallyCorrect() const override = 0;
  [[nodiscard]] bool IsSemanticallyCorrect() const override = 0;
};

class ParentClause : public SuchThatClause {
 public:
  explicit ParentClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kParent, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class ParentTClause : public SuchThatClause {
 public:
  explicit ParentTClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kParentT, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class FollowsClause : public SuchThatClause {
 public:
  explicit FollowsClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kFollows, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class FollowsTClause : public SuchThatClause {
 public:
  explicit FollowsTClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kFollowsT, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class UsesClause : public SuchThatClause {
 public:
  explicit UsesClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kUses, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class ModifiesClause : public SuchThatClause {
 public:
  explicit ModifiesClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kModifies, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class CallsClause : public SuchThatClause {
 public:
  explicit CallsClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kCalls, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class CallsTClause : public SuchThatClause {
 public:
  explicit CallsTClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kCallsT, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class NextClause : public SuchThatClause {
 public:
  explicit NextClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kNext, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class NextTClause : public SuchThatClause {
 public:
  explicit NextTClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kNextT, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class AffectsClause : public SuchThatClause {
 public:
  explicit AffectsClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kAffects, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class AffectsTClause : public SuchThatClause {
 public:
  explicit AffectsTClause(QueryReference *first, QueryReference *second)
      : SuchThatClause(RsType::kAffectsT, first, second) {
  }
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};

class PatternClause : public QueryClause {
 private:
  SynonymReference *synonym_declaration_;

  QueryReference *ent_ref_;

  ExpressionSpec *expression_;
 protected:
 public:
  explicit PatternClause(SynonymReference *synonym_declaration, QueryReference *ent_ref, ExpressionSpec *expression)
      : QueryClause(ClauseType::kPattern), synonym_declaration_(synonym_declaration), ent_ref_(ent_ref), expression_(expression) {
  };
  void setSynonymDeclaration(SynonymReference *synonym_declaration);
  [[nodiscard]] SynonymReference *getSynonymDeclaration() const;
  [[nodiscard]] QueryReference *getEntRef() const;
  [[nodiscard]] ExpressionSpec *getExpression() const;

  bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool IsSemanticallyCorrect() const override;
};