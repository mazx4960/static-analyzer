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
  void setRelationship(Relationship &);
  void setSynonym(QuerySynonym &);

  bool hasRelationship() const;
  bool hasSynonym() const;

  VariableEntity getEntityType() const;
  Relationship getRelationship() const;
  QuerySynonym getSynonym() const;


  // TODO(howtoosee): add methods for pattern
};

class PkbVariableQuery : public PKBQuery {};

class PkbAssignStmtQuery : public PKBQuery {};
