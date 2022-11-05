#include "context.h"
#include <cmath>
#include "query_optimiser.h"

Query QueryOptimiser::Optimise(const Query& query, Context *context, IPKBQuerier *pkb) {
  auto declarations = query.getSynonymDeclarations();
  auto *query_call = query.getQueryCall();
  auto clauses = query.getClauses();

  this->simple_stats_ = pkb->getSimpleStats();
  this->context_ = context;


  updateSynonymUsage(clauses);
  updateInitialWeights(clauses);
  updateSynonymWeight(clauses);

  ClauseVector new_clauses;
  std::sort(clauses.begin(), clauses.end(), clause_comparator_);
  new_clauses.push_back(clauses.back());
  clauses.pop_back();

  int iter_size = clauses.size();
  for (int i = 0; i < iter_size; ++i) {
    updateClauseWeight(clauses);
    updateSynonymWeight(clauses);
    std::sort(clauses.begin(), clauses.end(), clause_comparator_);
    new_clauses.push_back(clauses.back());
    clauses.pop_back();
  }

  return Query(declarations, query_call, new_clauses);
}

void QueryOptimiser::updateSynonymUsage(const ClauseVector &clauses) {
  for (auto *clause : clauses) {
    updateSynonymUsage(clause);
  }
}

void QueryOptimiser::updateSynonymUsage(QueryClause *clause) {
  auto clause_type = clause->getClauseType();
  if (clause_type == ClauseType::kSuchThat) {
    auto *such_that_clause = static_cast<SuchThatClause *>(clause);
    updateSynonymUsage(such_that_clause->getFirst());
    updateSynonymUsage(such_that_clause->getSecond());
    return;
  }

  if (clause_type == ClauseType::kPattern) {
    auto *pattern_clause = static_cast<PatternClause *>(clause);
    updateSynonymUsage(pattern_clause->getStmtRef());
    updateSynonymUsage(pattern_clause->getEntRef());
    return;
  }

  if (clause_type == ClauseType::kWith) {
    auto *with_clause = static_cast<WithClause *>(clause);
    updateSynonymUsage(with_clause->getFirst());
    updateSynonymUsage(with_clause->getSecond());
  }
}

void QueryOptimiser::updateSynonymUsage(QueryReference *query_reference) {
  if (query_reference->getRefType() != ReferenceType::kSynonym
      && query_reference->getRefType() != ReferenceType::kAttr) {
    return;
  }
  auto *synonym_ref = static_cast<ElemReference *>(query_reference);
  if (synonym_usage_.find(synonym_ref->getSynonym()) == synonym_usage_.end()) {
    this->synonym_usage_[synonym_ref->getSynonym()] = 0;
  }
  this->synonym_usage_[synonym_ref->getSynonym()]++;
}


int QueryOptimiser::getReferenceUsage(QueryReference *query_reference) {
  if (query_reference->getRefType() != ReferenceType::kSynonym
      && query_reference->getRefType() != ReferenceType::kAttr) {
    return 0;
  }
  auto *synonym_ref = static_cast<ElemReference *>(query_reference);
  return this->synonym_usage_[synonym_ref->getSynonym()];
}

EntitySet QueryOptimiser::getReferenceContext(QueryReference *query_reference) {
  if (query_reference->getRefType() != ReferenceType::kSynonym
      && query_reference->getRefType() != ReferenceType::kAttr) {
    return EntitySet();
  }
  auto *synonym_ref = static_cast<ElemReference *>(query_reference);
  return context_->Get(synonym_ref->getSynonym());
}

QueryClause *QueryOptimiser::updateInitialWeight(QueryClause *clause) {
  clause->setWeight(calculateInitialClauseWeight(clause));
  return clause;
}
void QueryOptimiser::updateInitialWeights(const ClauseVector& clauses) {
  for (auto *clause : clauses) {
    updateInitialWeight(clause);
  }
}

void QueryOptimiser::updateSynonymWeight(const ClauseVector& clauses) {
  for (auto *clause:clauses) {
    updateSynonymWeight(clause);
  }
}

void QueryOptimiser::updateSynonymWeight(QueryClause *query_clause) {
  auto clause_type = query_clause->getClauseType();
  if (clause_type == ClauseType::kSuchThat) {
    auto *such_that_clause = static_cast<SuchThatClause *>(query_clause);
    if (such_that_clause->getWeight() > such_that_clause->getFirst()->GetWeight()) {
      such_that_clause->getFirst()->SetWeight(such_that_clause->getWeight());
    }
    if (such_that_clause->getWeight() > such_that_clause->getSecond()->GetWeight()) {
      such_that_clause->getSecond()->SetWeight(such_that_clause->getWeight());
    }
  }
  if (clause_type == ClauseType::kPattern) {
    auto *pattern_clause = static_cast<PatternClause *>(query_clause);
    if (pattern_clause->getWeight() > pattern_clause->getStmtRef()->GetWeight()) {
      pattern_clause->getStmtRef()->SetWeight(pattern_clause->getWeight());
    }
    if (pattern_clause->getWeight() > pattern_clause->getEntRef()->GetWeight()) {
      pattern_clause->getEntRef()->SetWeight(pattern_clause->getWeight());
    }
  }
  if (clause_type == ClauseType::kWith) {
    auto *with_clause = static_cast<WithClause *>(query_clause);
    if (with_clause->getWeight() > with_clause->getFirst()->GetWeight()) {
      with_clause->getFirst()->SetWeight(with_clause->getWeight());
    }
    if (with_clause->getWeight() > with_clause->getSecond()->GetWeight()) {
      with_clause->getSecond()->SetWeight(with_clause->getWeight());
    }
  }
}

double QueryOptimiser::calculateInitialClauseWeight(QueryClause *query_clause) {

  auto clause_type = query_clause->getClauseType();
  if (clause_type == ClauseType::kSuchThat) {
    auto *such_that_clause = static_cast<SuchThatClause *>(query_clause);

    auto such_that_type = such_that_clause->getSuchThatType();

    auto first_usage = getReferenceUsage(such_that_clause->getFirst());
    auto second_usage = getReferenceUsage(such_that_clause->getSecond());

    int first_context_size = getReferenceContext(such_that_clause->getFirst()).size();
    int second_context_size = getReferenceContext(such_that_clause->getSecond()).size();

    // clause with 0 or 1 synonym
    if (first_usage == 0 || second_usage == 0) {
      return second_usage == first_usage ? -1 : 0;
    }

    // clause with 2 synonym with empty context (false clause)
    if (first_context_size == 0 || second_context_size == 0) {
      return -10;
    }

    if (such_that_type == RsType::kAffectsT) {
      int total_assign_stmt = simple_stats_[EntityType::kAssignStmt];
      // Value between 1 - 4
      return pow(2 - (1.0
          / (std::min(total_assign_stmt, first_context_size) * std::min(total_assign_stmt, second_context_size))), 2);
    }
    if (such_that_type == RsType::kAffects) {
      int total_assign_stmt = simple_stats_[EntityType::kAssignStmt];
      // Value between 1 - 2
      return pow(2 - (1.0
          / (std::min(total_assign_stmt, first_context_size) * std::min(total_assign_stmt, second_context_size))), 1);
    }
    if (such_that_type == RsType::kNextT) {
      int total_stmt = simple_stats_[EntityType::kStatement];
      // Value between 1 - 4
      return pow(2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_stmt, second_context_size))), 2);

    }
    if (such_that_type == RsType::kNext) {
      int total_stmt = simple_stats_[EntityType::kStatement];
      // Value between 1 - 2
      return pow(2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_stmt, second_context_size))), 1);

    }
    if (such_that_type == RsType::kParentT) {
      int total_stmt = simple_stats_[EntityType::kStatement];
      // Value between 1 - 2
      return pow(2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_stmt, second_context_size))), 1);

    }
    if (such_that_type == RsType::kParent) {
      int total_stmt = simple_stats_[EntityType::kStatement];
      // Value between 1 - 2
      return pow(2 - (1.0 / std::max(first_context_size, second_context_size)), 1);

    }
    if (such_that_type == RsType::kCallsT) {
      int total_procedure = simple_stats_[EntityType::kProcedure];
      double call_multiplier = (1.0 * simple_stats_[EntityType::kCallStmt]) / total_procedure;
      // Value between 1 - 2
      return pow(2 - (1.0 / std::max(1.0,
                                     (std::min(total_procedure, first_context_size) * call_multiplier)
                                         * (std::min(total_procedure, second_context_size) * call_multiplier))), 1);

    }
    if (such_that_type == RsType::kCalls) {
      // Value between 1 - 2
      return pow(2 - (1.0 / std::max(1,
                                     std::min(std::max(first_context_size, second_context_size),
                                              simple_stats_[EntityType::kCallStmt]))), 1);
    }
    if (such_that_type == RsType::kModifies) {
      int total_stmt = simple_stats_[EntityType::kIfStmt] + simple_stats_[EntityType::kWhileStmt]
          + simple_stats_[EntityType::kReadStmt] + simple_stats_[EntityType::kAssignStmt]
          + simple_stats_[EntityType::kCallStmt] + simple_stats_[EntityType::kProcedure];
      int total_variables = simple_stats_[EntityType::kVariable];
      // Value between 1 - 2
      return pow(
          2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_variables, second_context_size))), 1.5);

    }
    if (such_that_type == RsType::kUses) {
      int total_stmt = simple_stats_[EntityType::kIfStmt] + simple_stats_[EntityType::kWhileStmt]
          + simple_stats_[EntityType::kPrintStmt] + simple_stats_[EntityType::kAssignStmt]
          + simple_stats_[EntityType::kCallStmt] + simple_stats_[EntityType::kProcedure];
      int total_variables = simple_stats_[EntityType::kVariable];
      // Value between 1 - 2
      return pow(
          2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_variables, second_context_size))), 1.5);

    }
    if (such_that_type == RsType::kFollowsT) {
      int total_stmt = simple_stats_[EntityType::kStatement];
      // Value between 1 - 2
      return pow(2 - (1.0 / (std::min(total_stmt, first_context_size) * std::min(total_stmt, second_context_size))), 1);

    }
    if (such_that_type == RsType::kFollows) {
      // Value between 1 - 2
      return pow(2 - (1.0 / std::min(first_context_size, second_context_size)), 1);
    }
  }

  if (clause_type == ClauseType::kPattern) {
    int total_assign_stmt = simple_stats_[EntityType::kAssignStmt];
    int total_if_stmt = simple_stats_[EntityType::kIfStmt];
    int total_while_stmt = simple_stats_[EntityType::kWhileStmt];

    int total_variables = simple_stats_[EntityType::kVariable];
    int total_consts = simple_stats_[EntityType::kConstant];
    auto *pattern_clause = static_cast<PatternClause *>(query_clause);

    auto first_usage = getReferenceUsage(pattern_clause->getStmtRef());
    auto second_usage = getReferenceUsage(pattern_clause->getEntRef());

    int first_context_size = getReferenceContext(pattern_clause->getStmtRef()).size();
    int second_context_size = getReferenceContext(pattern_clause->getEntRef()).size();

    // pattern with 0 or 1 synonym
    if (first_usage == 0 || second_usage == 0) {
      return first_usage == second_usage ? -1 : 0;
    }

    // pattern with 2 synonym with empty context (false clause)
    if (first_context_size == 0 || second_context_size == 0) {
      return -10;
    }

    return pow(2 - (1.0
        / (std::min(std::max(std::max(total_assign_stmt, total_if_stmt), total_while_stmt), first_context_size)
            * std::min(std::max(total_consts, total_variables), second_context_size))), 1);
  }

  if (clause_type == ClauseType::kSuchThat) {
    auto *with_clause = static_cast<WithClause *>(query_clause);

    auto first_usage = getReferenceUsage(with_clause->getFirst());
    auto second_usage = getReferenceUsage(with_clause->getSecond());

    int first_context_size = getReferenceContext(with_clause->getFirst()).size();
    int second_context_size = getReferenceContext(with_clause->getSecond()).size();

    // clause with 0 or 1 synonym
    if (first_usage == 0 || second_usage == 0) {
      return first_usage == second_usage ? -1 : 0;
    }

    // clause with 2 synonym with empty context (false clause)
    if (first_context_size == 0 || second_context_size == 0) {
      return -10;
    }

    return pow(2 - (1.0 / std::min(first_context_size, second_context_size)), 1);


  }
  return -100;
}
void QueryOptimiser::updateClauseWeight(const ClauseVector& clauses) {
  for (auto *clause:clauses) {
    updateClauseWeight(clause);
  }
}
void QueryOptimiser::updateClauseWeight(QueryClause *clause) {
  auto clause_type = clause->getClauseType();
  if (clause_type == ClauseType::kSuchThat) {
    auto *such_that_clause = static_cast<SuchThatClause *>(clause);
    such_that_clause->setWeight((such_that_clause->getFirst()->GetWeight() + such_that_clause->getSecond()->GetWeight()) / 2.0);
  }
  if (clause_type == ClauseType::kPattern) {
    auto *pattern_clause = static_cast<PatternClause *>(clause);
    pattern_clause->setWeight((pattern_clause->getStmtRef()->GetWeight() + pattern_clause->getEntRef()->GetWeight()) / 2.0);

  }
  if (clause_type == ClauseType::kWith) {
    auto *with_clause = static_cast<WithClause *>(clause);
    with_clause->setWeight((with_clause->getFirst()->GetWeight() + with_clause->getSecond()->GetWeight()) / 2.0);
  }
}