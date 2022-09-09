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

 protected:
  explicit PKBEntityQuery(Entity &entity) : entity_type_(entity.GetType()) {};
  explicit PKBEntityQuery(EntityType entity_type) : entity_type_(entity_type) {};

 public:
  static PKBEntityQuery *getQuery(Entity &);

  [[nodiscard]] EntityType getEntityType() const;
};

/*
 * Subclasses for entity queries.
 */
class PKBProcedureQuery : public PKBEntityQuery {
 public:
  explicit PKBProcedureQuery() : PKBEntityQuery(EntityType::kProcedure) {};
  explicit PKBProcedureQuery(Entity &entity) : PKBEntityQuery(entity) {};
};

class PKBVariableQuery : public PKBEntityQuery {
 public:
  explicit PKBVariableQuery() : PKBEntityQuery(EntityType::kVariable) {};
  explicit PKBVariableQuery(Entity &entity) : PKBEntityQuery(entity) {};
};

class PKBStatementQuery : public PKBEntityQuery {
 public:
  explicit PKBStatementQuery() : PKBEntityQuery(EntityType::kStatement) {};
  explicit PKBStatementQuery(Entity &entity) : PKBEntityQuery(entity) {};

};

class PKBConstantQuery : public PKBEntityQuery {
 public:
  explicit PKBConstantQuery() : PKBEntityQuery(EntityType::kConstant) {};
  explicit PKBConstantQuery(Entity &entity) : PKBEntityQuery(entity) {};
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