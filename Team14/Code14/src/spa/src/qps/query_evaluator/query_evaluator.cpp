#include "query_evaluator.h"

std::unordered_set<QueryDeclaration *> QueryEvaluator::copyDeclarations() {
  for (auto *declaration : query_.getDeclarations()) {
    this->declarations_.insert({declaration->getSynonym(), declaration});
  }
  return this->getDeclarationAsSet();
}

std::unordered_set<QueryDeclaration *> QueryEvaluator::fetchContext() {
  this->copyDeclarations();

  std::unordered_set<Entity *> query_declaration_context_set;

  for (auto &it : this->declarations_) {
    QueryDeclaration *declaration = it.second;

    DeclarationType declaration_type = declaration->getType();
    if (DeclarationTypeAdaptor::canConvertToEntityType(declaration_type)) {
      EntityType entity_type = DeclarationTypeAdaptor::toEntityType(declaration_type);
      query_declaration_context_set = this->pkb_->getEntities(entity_type);
    } else if (DeclarationTypeAdaptor::canConvertToStatementType(declaration_type)) {
      StmtType stmt_type = DeclarationTypeAdaptor::toStatementType(declaration_type);
      query_declaration_context_set = this->pkb_->getEntities(stmt_type);
    } else {
      throw DeclarationTypeAdaptError(
          "DeclarationType cannot be converted to EntityType or StmtType, context cannot be fetched"
      );
    }
    declaration->setContext(query_declaration_context_set);
  }

  return this->getDeclarationAsSet();
}

std::unordered_set<QueryDeclaration *> QueryEvaluator::evaluateSubQueries() {
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (QueryClause subquery_clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, subquery_clause);
    std::unordered_set<Entity *> subquery_results_set = subquery_evaluator.evaluate();
  }

  return this->getDeclarationAsSet();
}

Result QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();

  // If there are no sub-queries, return the context of the selected query declaration.
  // Else, evaluate sub-queries first.
  if (this->query_.hasSubClauses()) {
    this->evaluateSubQueries();
  }

  QuerySynonym synonym = called_declaration->getSynonym();
  std::unordered_set<Entity *> context = called_declaration->getContext();

  return Result(synonym, context);
}

std::unordered_set<QueryDeclaration *> QueryEvaluator::getDeclarationAsSet() {
  std::unordered_set<QueryDeclaration *> declaration_set;

  for (auto &it : this->declarations_) {
    QueryDeclaration *declaration = it.second;
    declaration_set.insert(declaration);
  }

  return declaration_set;
}
