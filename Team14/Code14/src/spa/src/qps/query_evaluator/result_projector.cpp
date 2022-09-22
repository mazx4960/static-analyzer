#include "result_projector.h"

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
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(),
                  [](SubqueryResult subquery_result) { return subquery_result.empty(); })) {
    return EntityPointerUnorderedSet();
  }

  EntityPointerUnorderedSet candidates = this->called_declaration_->getContext();
  QuerySynonym *synonym = this->called_declaration_->getSynonym();

  switch (this->subquery_results_.size()) {
    case 0: {
      // Just return all possible results
      return candidates;
    }
    case 1: {
      if (this->subquery_results_[0].uses(this->called_declaration_)) {
        return this->subquery_results_[0].GetColumn(synonym);
      }
      return candidates;
    }
    case 2: {
      std::vector<QueryDeclaration *> common_synonyms = this->subquery_results_[0].getCommonSynonyms(this->subquery_results_[1]);
      switch (common_synonyms.size()) {
        case 0: {
          for (auto res : this->subquery_results_) {
            if (res.uses(this->called_declaration_)) {
              candidates = ResultProjector::intersect(candidates, res.GetColumn(synonym));
            }
          }
          return candidates;
        }
        case 1: {
          if (*common_synonyms[0]->getSynonym() == *synonym) {
            for (auto res : this->subquery_results_) {
              if (res.uses(this->called_declaration_)) {
                candidates = ResultProjector::intersect(candidates, res.GetColumn(synonym));
              }
            }
            return candidates;
          }
          SubqueryResult join = this->subquery_results_[0].Join(this->subquery_results_[1]);
          if (join.uses(this->called_declaration_)) { return join.GetColumn(synonym); }
          return this->called_declaration_->getContext();
        }
        case 2: {
          SubqueryResult intersection = this->subquery_results_[0].Intersect(this->subquery_results_[1]);
          if (intersection.uses(this->called_declaration_)) { return intersection.GetColumn(synonym); }
          return candidates;
        }
      }
    }
  }
  return EntityPointerUnorderedSet();
}
