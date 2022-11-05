#include <spdlog/spdlog.h>
#include "query_evaluator.h"

void QueryEvaluator::InitContext() {
  auto declarations = query_.getSynonymDeclarations();
  for (auto *synonym : declarations) {
    auto results = this->pkb_->getEntities(synonym->GetEntityType());
    ctx_->Set(synonym, results);
  }
}

void QueryEvaluator::EvaluateSubQueries() {
  std::vector<QueryClause *> subquery_clauses = query_.getClauses();
  for (auto *clause : subquery_clauses) {
    auto *strategy = EvaluationStrategy::getStrategy(this->pkb_, clause);
    Table *table = strategy->evaluate(this->ctx_);
    database_->AddTable(table);
  }
}

Result *QueryEvaluator::Evaluate() {
  // Evaluate sub-queries, get individual result tables.
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  this->InitContext();
  this->query_ = QueryOptimiser().Optimise(this->query_, this->ctx_, this->pkb_);
  this->EvaluateSubQueries();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to fetch results from pkb: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
  // display stats
  this->pkb_->LogStatistics();
  this->ctx_->LogStatistics();

  // Merge tables.
  begin = std::chrono::steady_clock::now();
  this->database_->MergeTables();
  end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to merge tables: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

  // Project results
  auto *select_call = this->query_.getQueryCall();
  ResultProjector *result_projector = ResultProjector::NewProjector(select_call, database_);
  auto *result = result_projector->Project();

  return result;
}

