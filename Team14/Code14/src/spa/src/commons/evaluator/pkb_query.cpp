#include "pkb_query.h"

void PKBQuery::setRelationship(QueryRelationship &rs) {
  this->has_relationship_ = true;
  this->rs_ = &rs;
}
void PKBQuery::setSynonym(Synonym &syn) {
  this->has_synonym_ = true;
  this->syn_ = &syn;
}
bool PKBQuery::hasRelationship() {
  return this->has_relationship_;
}

bool PKBQuery::hasSynonym() {
  return this->has_synonym_;
}

QueryRelationship PKBQuery::getRelationship() {
  return *this->rs_;
}
Synonym PKBQuery::getSynonym() {
  return *this->syn_;
}
