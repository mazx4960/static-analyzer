// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/relationship.h"
#include "qps/pql/query_synonym/query_synonym.h"

/*
 * Object to be sent to PKB for query.
 */
class PKBQuery {
 private:
  EntityType entity_type_;

  bool has_relationship_ = false;

  Relationship *rs_;

  bool has_synonym_ = false;

  QuerySynonym *syn_;

  // TODO(howtoosee): add fields for pattern

 protected:
  explicit PKBQuery(Entity &entity) : entity_type_(entity.GetType()) {};
  explicit PKBQuery(EntityType entity_type) : entity_type_(entity_type) {};

 public:
  static PKBQuery *getQuery(Entity &);

  void setRelationship(Relationship &);
  void setSynonym(QuerySynonym &);

  [[nodiscard]] bool hasRelationship() const;
  [[nodiscard]] bool hasSynonym() const;

  [[nodiscard]] EntityType getEntityType() const;
  [[nodiscard]] Relationship getRelationship() const;
  [[nodiscard]] QuerySynonym getSynonym() const;

  // TODO(howtoosee): add methods for pattern
};

class PKBProcedureQuery : public PKBQuery {
 public:
  explicit PKBProcedureQuery() : PKBQuery(EntityType::kProcedure) {};
  explicit PKBProcedureQuery(Entity &entity) : PKBQuery(entity) {};
};

class PKBVariableQuery : public PKBQuery {
 public:
  explicit PKBVariableQuery() : PKBQuery(EntityType::kVariable) {};
  explicit PKBVariableQuery(Entity &entity) : PKBQuery(entity) {};
};

class PKBStatementQuery : public PKBQuery {
 public:
  explicit PKBStatementQuery() : PKBQuery(EntityType::kStatement) {};
  explicit PKBStatementQuery(Entity &entity) : PKBQuery(entity) {};

};

class PKBConstantQuery : public PKBQuery {
 public:
  explicit PKBConstantQuery() : PKBQuery(EntityType::kConstant) {};
  explicit PKBConstantQuery(Entity &entity) : PKBQuery(entity) {};
};
