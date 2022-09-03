#pragma once

#include "qps/qpl/query_relationship/query_relationship.h"
#include "qps/qpl/synonym/synonym.h"

/*
 * Object to be sent to PKB for query.
 */
class PKBQuery {
 private:
  bool has_relationship_ = false;

  QueryRelationship *rs_;

  bool has_synonym_ = false;

  Synonym *syn_;

  // TODO: add fields for pattern


 public:
  PKBQuery() = default;

  void setRelationship(QueryRelationship &rs);
  bool hasRelationship();
  QueryRelationship getRelationship();

  void setSynonym(Synonym &syn);
  bool hasSynonym();
  Synonym getSynonym();

  // TODO: add methods for pattern
};

class PkbVariableQuery : public PKBQuery {};

class PkbAssignStmtQuery : public PKBQuery {};
