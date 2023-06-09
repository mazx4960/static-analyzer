// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "query_reference.h"
#include "expression_spec.h"

class QueryClause : public ICheckSemantics {
 private:
  ClauseType clause_type_;
  double weight_ = 0;

 protected:
  explicit QueryClause(ClauseType clauseType) : clause_type_(clauseType) {
  };

 public:
  [[nodiscard]] ClauseType getClauseType() const;
  [[nodiscard]] bool isSemanticallyCorrect() const override = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  void setWeight(double weight);
  [[nodiscard]] double getWeight() const;
  virtual bool operator<(const QueryClause &other) const;
  virtual bool operator==(const QueryClause &other) const = 0;
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
  static SuchThatClause *Create(RsType type, QueryReference *first, QueryReference *second);
  [[nodiscard]] RsType getSuchThatType() const;
  [[nodiscard]] QueryReference *getFirst() const;
  [[nodiscard]] QueryReference *getSecond() const;
  [[nodiscard]] bool isSemanticallyCorrect() const override = 0;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryClause &other) const override;
};

class ParentClause : public SuchThatClause {
 public:
  explicit ParentClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kParent,
                                                                                        first,
                                                                                        second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class ParentTClause : public SuchThatClause {
 public:
  explicit ParentTClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kParentT,
                                                                                         first,
                                                                                         second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class FollowsClause : public SuchThatClause {
 public:
  explicit FollowsClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kFollows,
                                                                                         first,
                                                                                         second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class FollowsTClause : public SuchThatClause {
 public:
  explicit FollowsTClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kFollowsT,
                                                                                          first,
                                                                                          second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class UsesClause : public SuchThatClause {
 public:
  explicit UsesClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kUses, first, second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class ModifiesClause : public SuchThatClause {
 public:
  explicit ModifiesClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kModifies,
                                                                                          first,
                                                                                          second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class CallsClause : public SuchThatClause {
 public:
  explicit CallsClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kCalls, first, second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class CallsTClause : public SuchThatClause {
 public:
  explicit CallsTClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kCallsT,
                                                                                        first,
                                                                                        second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class NextClause : public SuchThatClause {
 public:
  explicit NextClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kNext, first, second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class NextTClause : public SuchThatClause {
 public:
  explicit NextTClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kNextT, first, second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class AffectsClause : public SuchThatClause {
 public:
  explicit AffectsClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kAffects,
                                                                                         first,
                                                                                         second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class AffectsTClause : public SuchThatClause {
 public:
  explicit AffectsTClause(QueryReference *first, QueryReference *second) : SuchThatClause(RsType::kAffectsT,
                                                                                          first,
                                                                                          second) {
  }
  [[nodiscard]] bool isSemanticallyCorrect() const override;
};

class PatternClause : public QueryClause {
 private:
  SynonymReference *stmt_ref_;

  QueryReference *ent_ref_;

  ExpressionSpec *first_expression_;

  ExpressionSpec *second_expression_;
 protected:
 public:
  explicit PatternClause(SynonymReference *synonym_declaration,
                         QueryReference *ent_ref,
                         ExpressionSpec *first_expression,
                         ExpressionSpec *second_expression)
      : QueryClause(ClauseType::kPattern),
        stmt_ref_(synonym_declaration),
        ent_ref_(ent_ref),
        first_expression_(first_expression),
        second_expression_(second_expression) {
  };
  [[nodiscard]] SynonymReference *getStmtRef() const;
  [[nodiscard]] QueryReference *getEntRef() const;
  [[nodiscard]] ExpressionSpec *getFirstExpression() const;
  [[nodiscard]] ExpressionSpec *getSecondExpression() const;
  [[nodiscard]] bool isSemanticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryClause &other) const override;
};

class WithClause : public QueryClause {
 private:
  Comparator comparator_;
  QueryReference *first_;
  QueryReference *second_;

 public:
  explicit WithClause(Comparator comparator, QueryReference *first, QueryReference *second)
      : QueryClause(ClauseType::kWith), comparator_(comparator), first_(first), second_(second) {
  };
  [[nodiscard]] Comparator getComparator() const;
  [[nodiscard]] QueryReference *getFirst() const;
  [[nodiscard]] QueryReference *getSecond() const;
  [[nodiscard]] bool isSemanticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryClause &other) const override;
};

struct QueryClauseUniquePointerComparator {
  bool operator()(const QueryClause *lhs, const QueryClause *rhs) const {
    return !((*lhs) == (*rhs)) ;
  }
};