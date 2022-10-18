#pragma once

#include <utility>

#include "evaluation_strategy.h"
#include "commons/entity.h"
#include "qps/result.h"
#include "qps/pql/query_reference.h"
#include "subquery_result.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class ResultProjector {
 private:
  std::vector<ElemReference *> called_declarations_;

  IPKBQuerier *pkb_;

  std::vector<SubqueryResult> subquery_results_;

  static EntityPointerUnorderedSet intersect(const EntityPointerUnorderedSet &first,
                                             const EntityPointerUnorderedSet &second);

 public:
  ResultProjector(std::vector<ElemReference *> declarations,
                  std::vector<SubqueryResult> subquery_results,
                  IPKBQuerier *pkb)
      : called_declarations_(std::move(declarations)), subquery_results_(std::move(subquery_results)), pkb_(pkb) {
  };

  SubqueryResult project();
};
