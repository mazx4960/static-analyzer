#include "result_projector.h"

#include "spdlog/spdlog.h"

/**
 * Intersect the context of two QueryDeclarations.
 * @param first first set of Entity pointers.
 * @param second second set of Entity pointers.
 * @return intersection of sets of Entity pointers.
 */
EntityPointerUnorderedSet ResultProjector::intersect(const EntityPointerUnorderedSet &first, const EntityPointerUnorderedSet &second) {
  EntityPointerUnorderedSet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) { result.insert(entity); }
  }
  return result;
}

/**
 * Project results from list of subquery results.
 * @return set of Entity pointers of final result.
 */
EntityPointerUnorderedSet ResultProjector::project() {

  EntityPointerUnorderedSet candidates = this->called_declaration_->getContext();
  QuerySynonym *synonym = this->called_declaration_->getSynonym();
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(),
                  [](SubqueryResult subquery_result) {
                    return subquery_result.IsEmpty();
                  })) {
    spdlog::debug("Some table is empty");
    return EntityPointerUnorderedSet();
  }
  if (std::none_of(subquery_results_.begin(), subquery_results_.end(),
                  [synonym](SubqueryResult subquery_result) {
                    return subquery_result.Uses(synonym);
                  })) {
    spdlog::debug("No tables use the synonym");
    return pkb_->getEntities(called_declaration_->getType());
  }

  SubqueryResult intermediate_result = subquery_results_[0];
  for (auto result = subquery_results_.begin() + 1; result != subquery_results_.end(); result++) {
    intermediate_result = intermediate_result.Join(*result);
  }
  return intermediate_result.GetColumn(synonym);
}
