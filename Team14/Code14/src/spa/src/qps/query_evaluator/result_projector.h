#pragma once

#include "evaluation_strategy.h"
#include "commons/entity.h"
#include "commons/result.h"
#include "qps/pql/query_declaration.h"
#include "subquery_result.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class ResultProjector {
 private:
  QueryDeclaration *called_declaration_;

  std::vector<SubqueryResult> subquery_results_;

  static EntityPointerUnorderedSet intersect(const EntityPointerUnorderedSet &first, const EntityPointerUnorderedSet &second);

 public:
  ResultProjector(QueryDeclaration *declaration, std::vector<SubqueryResult> subquery_results)
      : called_declaration_(declaration), subquery_results_(std::move(subquery_results)) {};

  EntityPointerUnorderedSet project();
};
