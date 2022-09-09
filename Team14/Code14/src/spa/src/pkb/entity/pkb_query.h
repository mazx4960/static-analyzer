// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/relationship.h"
#include "qps/pql/query_clause/pattern.h"
#include "qps/pql/query_clause/query_clause.h"
#include "qps/pql/query_synonym/query_synonym.h"

/*
 * Object to be sent to PKB for query.
 */
class PKBQuery {
 protected:
  PKBQuery() = default;
};

/*
 * Class for entity query.
 */
class PKBEntityQuery : public PKBQuery {
 private:
  EntityType entity_type_;

  QuerySynonym synonym_;

 protected:
  explicit PKBEntityQuery(EntityType entity_type, QuerySynonym &synonym)
      : entity_type_(entity_type), synonym_(synonym) {};

 public:
  static PKBEntityQuery *getQuery(Entity &, QuerySynonym &);

  [[nodiscard]] EntityType getEntityType() const;
  [[nodiscard]] QuerySynonym getSynonym() const;
};

/*
 * Subclasses for entity queries.
 */
class PKBProcedureQuery : public PKBEntityQuery {
 public:
  explicit PKBProcedureQuery(QuerySynonym &synonym) : PKBEntityQuery(EntityType::kProcedure, synonym) {};
};

class PKBVariableQuery : public PKBEntityQuery {
 public:
  explicit PKBVariableQuery(QuerySynonym &synonym) : PKBEntityQuery(EntityType::kVariable, synonym) {};
};

class PKBStatementQuery : public PKBEntityQuery {
 public:
  explicit PKBStatementQuery(QuerySynonym &synonym) : PKBEntityQuery(EntityType::kStatement, synonym) {};

};

class PKBConstantQuery : public PKBEntityQuery {
 public:
  explicit PKBConstantQuery(QuerySynonym &synonym) : PKBEntityQuery(EntityType::kConstant, synonym) {};
};

/*
 * Class for clause query.
 */
class PKBClauseQuery : public PKBQuery {
  class PKBSuchThatQuery;
  class PKBPatternQuery;

 private:

  ClauseType clause_type_;

 protected:
  explicit PKBClauseQuery(ClauseType clause_type) : clause_type_(clause_type) {};
 public:
  [[nodiscard]] ClauseType getClauseType() const;
};

/*
 * Subclasses for clause queries.
 */
class PKBSuchThatQuery : public PKBClauseQuery {
 private:

  Relationship *rs_;

  explicit PKBSuchThatQuery(Relationship &rs) : PKBClauseQuery(ClauseType::kSuchThat), rs_(&rs) {};

 public:
  static PKBSuchThatQuery *getQuery(Relationship &);
  void setRelationship(Relationship &);
  [[nodiscard]] Relationship *getRelationship() const;
};

class PKBPatternQuery : public PKBClauseQuery {
 private:
  Pattern *pattern_;

  explicit PKBPatternQuery(Pattern &pattern) : PKBClauseQuery(ClauseType::kPattern), pattern_(&pattern) {};

 public:
  static PKBPatternQuery *getQuery(Pattern &);
  void setPattern(Pattern &);
  [[nodiscard]] Pattern *getPattern() const;
};

/*
 * PKBEntityQuery exceptions.
 */
class PKBQueryBuildingError : public std::runtime_error {
 public:
  explicit PKBQueryBuildingError(const std::string &message) : std::runtime_error(message) {};
};

class PKBQueryFetchError : public std::runtime_error {
 public:
  explicit PKBQueryFetchError(const std::string &message) : std::runtime_error(message) {};
};