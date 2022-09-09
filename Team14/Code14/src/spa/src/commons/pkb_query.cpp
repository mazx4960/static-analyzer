#include "pkb_query.h"

/*
 * Entity query factory
 */
PKBEntityQuery *PKBEntityQuery::getQuery(Entity &entity, QuerySynonym &synonym) {
  switch (entity.GetType()) {
    case EntityType::kProcedure:return new PKBProcedureQuery(synonym);
    case EntityType::kStatement:return new PKBStatementQuery(synonym);
    case EntityType::kVariable:return new PKBVariableQuery(synonym);
    case EntityType::kConstant:return new PKBConstantQuery(synonym);
    default: return nullptr;
  }
}

EntityType PKBEntityQuery::getEntityType() const {
  return this->entity_type_;
}
QuerySynonym PKBEntityQuery::getSynonym() const {
  return this->synonym_;
}

ClauseType PKBClauseQuery::getClauseType() const {
  return this->clause_type_;
}

PKBSuchThatQuery *PKBSuchThatQuery::getQuery(Relationship &rs) {
  return new PKBSuchThatQuery(rs);
}

void PKBSuchThatQuery::setRelationship(Relationship &rs) {
  this->rs_ = &rs;
}

Relationship *PKBSuchThatQuery::getRelationship() const {
  return this->rs_;
}

PKBPatternQuery *PKBPatternQuery::getQuery(Pattern &pattern) {
  return new PKBPatternQuery(pattern);
}

void PKBPatternQuery::setPattern(Pattern &pattern) {
  this->pattern_ = &pattern;
}

Pattern *PKBPatternQuery::getPattern() const {
  return this->pattern_;
}
