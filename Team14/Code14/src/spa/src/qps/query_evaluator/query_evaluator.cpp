#include <spdlog/spdlog.h>
#include "query_evaluator.h"

SynonymReferencePointerUnorderedSet QueryEvaluator::getDeclarationAsSet() {
  SynonymReferencePointerUnorderedSet declaration_set;

  std::copy(this->declarations_.begin(),
            this->declarations_.end(),
            std::inserter(declaration_set, declaration_set.begin()));
  return declaration_set;
}

SynonymReferencePointerUnorderedSet QueryEvaluator::copyDeclarations() {
  Query &query = this->query_;
  std::vector<SynonymReference *> query_declarations = query.getSynonymDeclarations();
  this->declarations_ = SynonymReferencePointerUnorderedSet(query_declarations.begin(), query_declarations.end());
  return this->getDeclarationAsSet();
}

SynonymReferencePointerUnorderedSet QueryEvaluator::fetchContext() {
  this->copyDeclarations();

  for (auto *declaration : this->declarations_) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> query_declaration_context_set;
    query_declaration_context_set = this->pkb_->getEntities(declaration->getEntityType());
    declaration->setContext(query_declaration_context_set);
  }
  return this->getDeclarationAsSet();
}

std::vector<SubqueryResult> QueryEvaluator::evaluateSubqueries() {
  std::vector<QueryClause *> subquery_clauses = this->query_.getClauses();
  std::vector<SubqueryResult> subquery_results_list;
  for (auto *clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, clause);
    SubqueryResult subquery_result = subquery_evaluator.evaluate();
    subquery_results_list.push_back(subquery_result);
  }
  return subquery_results_list;
}

Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Evaluate sub-queries, get individual result tables.
  std::vector<SubqueryResult> subquery_results = this->evaluateSubqueries();

  // Query declarations for whose subquery_results are to be returned.
  auto *select_call = this->query_.getQueryCall();

  switch (select_call->getSelectType()) {
    case SelectType::kElem: {
      spdlog::debug("Creating Result for Element select type");
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_declarations = elem_select_call->getReferences();
      auto *result_projector = new SelectProjector(called_declarations, subquery_results);
      result_projector->project();
      SubqueryResult result_context = result_projector->select_results();
      spdlog::debug("Element Result context size: {}", result_context.GetRows().size());
      return new ElemResult(called_declarations, result_context);
    }
    case SelectType::kBoolean: {
      spdlog::debug("Creating Result for Boolean select type");
      auto *result_projector = new BooleanProjector(subquery_results);
      result_projector->project();
      bool has_results = result_projector->has_results();
      spdlog::debug("Boolean Result non-empty? {}", has_results);
      return new BooleanResult(has_results);
    }
    default: {
      spdlog::debug("Invalid select type");
      return Result::empty();
    }
  }
}
