#include "pkb_query.h"

/*
 * Setters
 */
void PKBQuery::setEntityType(VariableEntity &entity_type) {
  this->entity_type_ = &entity_type;
}

void PKBQuery::setRelationship(Relationship &rs) {
  this->has_relationship_ = true;
  this->rs_ = &rs;
}

void PKBQuery::setSynonym(QuerySynonym &syn) {
  this->has_synonym_ = true;
  this->syn_ = &syn;
}

/*
 * Boolean checkers
 */
bool PKBQuery::hasRelationship() const {
  return this->has_relationship_;
}

bool PKBQuery::hasSynonym() const {
  return this->has_synonym_;
}

/*
 * Getters
 */
VariableEntity PKBQuery::getEntityType() const {
  return *this->entity_type_;
}

Relationship PKBQuery::getRelationship() const {
  return *this->rs_;
}

QuerySynonym PKBQuery::getSynonym() const {
  return *this->syn_;
}
