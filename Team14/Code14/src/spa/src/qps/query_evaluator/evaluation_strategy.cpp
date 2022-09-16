#include "evaluation_strategy.h"
#include "spdlog/spdlog.h"

/**
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause *query_clause) {
  switch (query_clause->getClauseType()) {
    case ClauseType::kSuchThat: {
      spdlog::debug("Creating SuchThatEvaluationStrategy");
      return new SuchThatStrategy(pkb, static_cast<SuchThatClause *>(query_clause));
    }
    case ClauseType::kPattern: {
      spdlog::debug("Creating PatternEvaluationStrategy");
      return new PatternStrategy(pkb, static_cast<PatternClause *>(query_clause));
    }
    default: throw EvaluationStrategyCreationError("Invalid query clause type");
  }
}

// TODO(howtoosee) implement SuchThat strategy
void SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  EntityPointerUnorderedSet first_param_context = first_param->getContext();
  EntityPointerUnorderedSet second_param_context = second_param->getContext();

  if (first_param_context.size() <= second_param_context.size()) {
    // Intersect 2nd set first
    intersectContext(first_param, second_param, rs_type, false);
    // Then intersect 1st set
    intersectContext(second_param, first_param, rs_type, true);

  } else {
    // Intersect 1st set first
    intersectContext(second_param, first_param, rs_type, true);
    // Then intersect 2nd set
    intersectContext(first_param, second_param, rs_type, false);
  }
}

void SuchThatStrategy::intersectContext(QueryDeclaration *param_to_send, QueryDeclaration *param_to_be_intersected,
                                        RsType rs_type, bool invert_search) {
  EntityPointerUnorderedSet valid_entities_for_other_param;

  for (auto *context_entity : param_to_send->getContext()) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByRelationship(rs_type, context_entity, invert_search);
    if (valid_entities.empty()) {
      param_to_send->removeEntityFromContext(*context_entity);
    } else {
      for (auto *entity : valid_entities) { valid_entities_for_other_param.insert(entity); }
    }
  }
  param_to_be_intersected->intersectContext(valid_entities_for_other_param);
}

void PatternStrategy::evaluate() {
  spdlog::debug("Evaluating Pattern clause");

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  QueryDeclaration *third_param = this->clause_->getThird();

  intersectContext(first_param, second_param, third_param);
}
void PatternStrategy::intersectContext(QueryDeclaration *assign_param, QueryDeclaration *left_param,
                                       QueryDeclaration *right_param) {
  std::string pattern_substring = right_param->toString();
  EntityPointerUnorderedSet assign_result;
  EntityPointerUnorderedSet intersected_results;

  // Assignment on the left is a Wildcard
  if (left_param->getType() == EntityType::kWildcard) {
    EntityPointerUnorderedSet all_variables = pkb_->getEntities(EntityType::kVariable);
    for (auto *variable_entity : all_variables) {
      assign_result.merge(pkb_->getByPattern(variable_entity, pattern_substring));
    }
    assign_param->intersectContext(assign_result);
  }

  // Assignment on the left is a SIMPLE entity
  if (left_param->getType() == EntityType::kString) {
    assign_param->intersectContext(pkb_->getByPattern(new VariableEntity(left_param->toString()), pattern_substring));
  }

  // Assignment on the left is a PQL declaration
  if (left_param->getType() == EntityType::kVariable) {
    for (auto *variable_entity : left_param->getContext()) {
      EntityPointerUnorderedSet valid_entities = pkb_->getByPattern(variable_entity, pattern_substring);
      if (valid_entities.empty()) {
        left_param->removeEntityFromContext(*variable_entity);
      } else {
        for (auto *assign_entity : valid_entities) { assign_result.insert(assign_entity); }
      }
    }
    assign_param->intersectContext(assign_result);
  }
  spdlog::info("Pattern intersection executed...");
  spdlog::debug("Intersected size: {}", assign_param->getContext().size());
}
