#pragma once

#include "commons/relationship.h"
#include "qps/pql/query_synonym/query_synonym.h"

/*
 * Object to be sent to PKB for query.
 */
class PKBQuery {
 private:
  VariableEntity *entity_type_;

  bool has_relationship_ = false;

  Relationship *rs_;

  bool has_synonym_ = false;

  QuerySynonym *syn_;

  // TODO: add fields for pattern


 public:
  PKBQuery() = default;

  void setEntityType(VariableEntity &);
  VariableEntity getEntityType();

  void setRelationship(Relationship &);
  bool hasRelationship();
  Relationship getRelationship();

  void setSynonym(QuerySynonym &);
  bool hasSynonym();
  QuerySynonym getSynonym();

  // TODO: add methods for pattern
};

class PkbVariableQuery : public PKBQuery {};

class PkbAssignStmtQuery : public PKBQuery {};
