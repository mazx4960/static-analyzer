// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"
#include "qps/pql/query_keywords.h"

QueryBuilder::QueryBuilder(Query *query_blueprint) {
  if (query_blueprint == nullptr) {
    throw ParseSemanticError("No query given");
  }
  this->query_ = query_blueprint;
}

Query *QueryBuilder::build() {
  this->declarations_ = buildDeclarations(query_->getSynonymDeclarations());
  buildQueryCall(query_->getQueryCall());
  buildClauses(query_->getClauses());
  return query_;
}

std::vector<SynonymReference *> QueryBuilder::buildDeclarations(const std::vector<SynonymReference *> &declaration_blueprints) {
  for (auto *declaration_blueprint : declaration_blueprints) {
    buildDeclaration(declaration_blueprint);
  }
  return declaration_blueprints;
}

SynonymReference *QueryBuilder::buildDeclaration(SynonymReference *declaration_blueprint) {
  if (declaration_blueprint == nullptr) {
    throw ParseSemanticError("Missing declaration");
  }
  if (!synonyms_.insert(declaration_blueprint->getReferenceValue()).second) {
    throw ParseSemanticError("Synonym already declared: " + declaration_blueprint->getReferenceValue());
  }
  return declaration_blueprint;
}

QueryCall *QueryBuilder::buildQueryCall(QueryCall *query_call_blueprint) {
  if (query_call_blueprint == nullptr) {
    throw ParseSemanticError("Missing Select call");
  }
  buildQueryCallElemReferences(query_call_blueprint->getReferences());
  return query_call_blueprint;
}

std::vector<ElemReference *> QueryBuilder::buildQueryCallElemReferences(std::vector<ElemReference *> query_call_reference_blueprint) {
  if (query_call_reference_blueprint.empty()) {
    throw ParseSemanticError("Missing declarations");
  }
  // Update reference by select from declarations
  for (auto &i : query_call_reference_blueprint) {
    auto *elem_ref = i;
    if (elem_ref->getRefType() == ReferenceType::kSynonym) {
      i = getDeclaration(static_cast<SynonymReference *>(elem_ref));
    }
    if (elem_ref->getRefType() == ReferenceType::kAttr) {
      auto *attr_ref = static_cast<AttrReference *>(elem_ref);
      attr_ref->setSynonymReference(getDeclaration(attr_ref->getSynonymReference()));
    }
  }
  return query_call_reference_blueprint;
}

SynonymReference *QueryBuilder::getDeclaration(SynonymReference *synonym_reference) {
  auto *synonym = synonym_reference->getSynonym();
  for (auto *declaration : declarations_) {
    QuerySynonym *declaration_synonym = declaration->getSynonym();
    if (*declaration_synonym == *synonym) {
      return declaration;
    }
  }
  if (QueryKeywords::isSpecialSynonymKeyword(synonym->toString())) {
    synonym_reference->setBooleanRef(true);
    return synonym_reference;
  }
  throw ParseSemanticError("Synonym is not declared: " + synonym->toString());
}

SynonymReference *QueryBuilder::getDeclaration(const QuerySynonym *synonym) {
  for (auto *declaration : declarations_) {
    QuerySynonym *declaration_synonym = declaration->getSynonym();
    if (*declaration_synonym == *synonym) {
      return declaration;
    }
  }
  if (QueryKeywords::isSpecialSynonymKeyword(synonym->toString())) {

  }
  throw ParseSemanticError("Synonym is not declared: " + synonym->toString());
}

std::vector<QueryClause *> QueryBuilder::buildClauses(const std::vector<QueryClause *> &clauses_blueprint) {
  for (auto *clause : clauses_blueprint) {
    switch (clause->getClauseType()) {
      case ClauseType::kPattern:
        buildPattern(static_cast<PatternClause *>(clause));
        break;
      case ClauseType::kSuchThat:
        buildSuchThat(static_cast<SuchThatClause *>(clause));
        break;
      case ClauseType::kWith:
        buildWith(static_cast<WithClause *>(clause));
      default:
        throw ParseSemanticError("Unsupported clause type!");
    }
  }
  return clauses_blueprint;
}

PatternClause *QueryBuilder::buildPattern(PatternClause *clause_blueprint) {
  SynonymReference *syn_assign = clause_blueprint->getSynonymDeclaration();
  QueryReference *ent_ref = clause_blueprint->getEntRef();
  ExpressionSpec *expression_spec = clause_blueprint->getExpression();
  if (syn_assign == nullptr || ent_ref == nullptr || expression_spec == nullptr) {
    throw ParseSemanticError("Missing parameters");
  }
  clause_blueprint->setSynonymReference(getDeclaration(syn_assign->getSynonym()));
  if (ent_ref->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setEntReference(getDeclaration(static_cast<SynonymReference *>(ent_ref)->getSynonym()));
  }
  if (!clause_blueprint->IsSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid pattern parameter type");
  }
  return clause_blueprint;
}

SuchThatClause *QueryBuilder::buildSuchThat(SuchThatClause *clause_blueprint) {
  QueryReference *first = clause_blueprint->getFirst();
  QueryReference *second = clause_blueprint->getSecond();
  if (first == nullptr || second == nullptr) {
    throw ParseSemanticError("Missing parameters!");
  }
  if (first->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setFirst(getDeclaration(static_cast<SynonymReference *>(first)->getSynonym()));
  }
  if (second->getRefType() == ReferenceType::kSynonym) {
    clause_blueprint->setSecond(getDeclaration(static_cast<SynonymReference *>(second)->getSynonym()));
  }
  if (!clause_blueprint->IsSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid such that parameter type");
  }
  return clause_blueprint;
}

WithClause *QueryBuilder::buildWith(WithClause *clause_blueprint) {
  QueryReference *first = clause_blueprint->getFirst();
  QueryReference *second = clause_blueprint->getSecond();
  if (first->getRefType() == ReferenceType::kAttr) {
    auto *first_attr_reference = static_cast<AttrReference *>(first);
    first_attr_reference->setSynonymReference(this->getDeclaration(first_attr_reference->getSynonymReference()));
    clause_blueprint->setFirst(first_attr_reference);
  }
  if (second->getRefType() == ReferenceType::kAttr) {
    auto *second_attr_reference = static_cast<AttrReference *>(second);
    second_attr_reference->setSynonymReference(this->getDeclaration(second_attr_reference->getSynonymReference()));
    clause_blueprint->setSecond(second_attr_reference);
  }
  if (!clause_blueprint->IsSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid with parameter type");
  }
  return clause_blueprint;
}
