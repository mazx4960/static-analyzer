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
SubqueryResult ResultProjector::project() {
  std::vector<QuerySynonym* > called_synonyms{};
  for (auto *elem_ref : called_declarations_) {
    called_synonyms.push_back(elem_ref->getSynonymReference()->getSynonym());
  }
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(),
                  [](SubqueryResult subquery_result) {
                    return subquery_result.IsEmpty();
                  })) {
    spdlog::debug("Some table is empty");
    return SubqueryResult::Empty(called_synonyms);
  }

  SubqueryResult intermediate_result = subquery_results_[0];
  for (auto result = subquery_results_.begin() + 1; result != subquery_results_.end(); result++) {
    intermediate_result = intermediate_result.Join(*result);
  }
  for (auto *decl : called_declarations_) {
    auto *synonym = decl->getSynonymReference()->getSynonym();
    if (!intermediate_result.Uses(synonym)) {
      intermediate_result = intermediate_result.AddColumn(synonym, decl->getContext());
    }
  }
  return intermediate_result.GetColumns(called_synonyms);
}
