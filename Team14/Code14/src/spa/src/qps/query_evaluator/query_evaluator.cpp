#include <spdlog/spdlog.h>
#include "query_evaluator.h"

void QueryEvaluator::InitContext() {
  auto declarations = query_.getSynonymDeclarations();
  for (auto *synonym : declarations) {
    auto results = this->pkb_->getEntities(synonym->GetEntityType());
    ctx_->Set(synonym, results);
  }
}

std::vector<SubqueryResult> QueryEvaluator::EvaluateSubqueries() {
  std::vector<QueryClause *> subquery_clauses = getSortedQueries();
  std::vector<SubqueryResult> subquery_results_list;
  for (auto *clause : subquery_clauses) {
    auto *strategy = EvaluationStrategy::getStrategy(this->pkb_, clause);
    SubqueryResult subquery_result = strategy->evaluate(this->ctx_);
    subquery_results_list.push_back(subquery_result);
  }
  return subquery_results_list;
}

Result *QueryEvaluator::Evaluate() {
  // Evaluate sub-queries, get individual result tables.
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  this->InitContext();
  std::vector<SubqueryResult> subquery_results = this->EvaluateSubqueries();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to fetch results from pkb: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

  // Query declarations for whose subquery_results are to be returned.
  begin = std::chrono::steady_clock::now();
  auto *select_call = this->query_.getQueryCall();

  ResultProjector *result_projector = ResultProjector::getProjector(select_call, subquery_results);
  auto *result = result_projector->project(this->ctx_);
  end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to project results: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
  
  // display pkb stats
  this->pkb_->LogStatistics();

  return result;
}

ClauseVector QueryEvaluator::getSortedQueries() {
  struct {
    bool operator()(QueryClause *a, QueryClause *b) const {
      return a->getWeight() < b->getWeight();
    }
  } comparator;

  auto clauses = this->query_.getClauses();
  for (auto *clause : clauses) {
    updateWeight(clause);
  }
  std::sort(clauses.begin(), clauses.end(), comparator);
  return clauses;
}

QueryClause *QueryEvaluator::updateWeight(QueryClause *clause) {
  switch (clause->getClauseType()) {
    case ClauseType::kSuchThat:
      clause->setWeight(calculateWeight(static_cast<SuchThatClause *>(clause)->getFirst()->getUsage(),
                                        static_cast<SuchThatClause *>(clause)->getSecond()->getUsage()));
      break;
    case ClauseType::kPattern:
      clause->setWeight(calculateWeight(static_cast<PatternClause *>(clause)->getStmtRef()->getUsage(),
                                        static_cast<PatternClause *>(clause)->getEntRef()->getUsage()));
      break;
    default:
      return clause;
  }
  return clause;
}


double QueryEvaluator::calculateWeight(int first_usage_count, int second_usage_count) {
  if (first_usage_count <= 0 && second_usage_count <= 0) {
    return -1;
  }
  return 1.0 / (std::max(first_usage_count, second_usage_count) * (first_usage_count + second_usage_count));
}
