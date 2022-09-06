#include "pkb_query.h"

/*
 * Factory
 */
PKBQuery *PKBQuery::getQuery(Entity &entity) {
  switch (entity.GetType()) {
    case EntityType::kProcedure:return new PKBProcedureQuery();
    case EntityType::kStatement:return new PKBStatementQuery();
    case EntityType::kVariable:return new PKBVariableQuery();
    case EntityType::kConstant:return new PKBConstantQuery();
    default: return nullptr;
  }

}

/*
 * Setters
 */
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
EntityType PKBQuery::getEntityType() const {
  return this->entity_type_;
}

Relationship PKBQuery::getRelationship() const {
  return *this->rs_;
}

QuerySynonym PKBQuery::getSynonym() const {
  return *this->syn_;
}
