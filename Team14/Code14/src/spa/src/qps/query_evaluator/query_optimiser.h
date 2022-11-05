#pragma once

#include <unordered_map>
#include "qps/pql/query.h"
#include "types.h"
#include "pkb/pkb.h"


class QueryOptimiser {
 private:
  Context *context_;
  std::unordered_map<EntityType, int> simple_stats_;
  std::unordered_map<QuerySynonym*, int> synonym_usage_;


 public:
  QueryOptimiser() = default;
  Query *Optimise(Query *query, Context *context, IPKBQuerier pkb);

  void updateSynonymUsage(QueryClause *clause);
  void updateSynonymUsage(const ClauseVector &clauses);
  void updateSynonymUsage(QueryReference *query_reference);
  int getReferenceUsage(QueryReference *query_reference);
  std::pair<int, int> getSynonymUsage(QueryClause *clause);
  EntitySet getReferenceContext(QueryReference *query_reference);
  double calculateInitialClauseWeight(QueryClause *query_clause);
  QueryClause *updateInitialWeight(QueryClause *clause);
  void updateClauseWeight(const ClauseVector& clauses);
  void updateClauseWeight(QueryClause *clause);
  void updateSynonymWeight(QueryClause *query_clause);
  void updateSynonymWeight(ClauseVector clauses);
  void updateInitialWeights(const ClauseVector &clauses);
};
struct {
  bool operator()(QueryClause *first, QueryClause *second) const {
    return first->getWeight() < second->getWeight();
  }
} clause_comparator;

inline std::function<double(int, int)> RsTypeToWeightFunction(RsType type) {
  switch (type) {
    case RsType::kFollows:
      return [](int x, int y) {
        int min = std::min(x, y);
        int max = std::max(x, y);
        double min_reduction = (min * 0.2);
        double max_reduction = (max - min) + min_reduction;
        double cost = std::min(min - min_reduction, max - max_reduction);
        return (std::max(max_reduction, min_reduction) * (max_reduction + min_reduction)) / cost;
      };
    case RsType::kFollowsT:
      return [](int x, int y) {
        int min = std::min(x, y);
        int max = std::max(x, y);
        double min_reduction = (min * 0.2);
        double max_reduction = (max - min);
        double cost = 2 * std::min(min - min_reduction, max - max_reduction);
        return (std::max(max_reduction, min_reduction) * (max_reduction + min_reduction)) / cost;
      };
    case RsType::kParent:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.8);
        double y_reduction = std::abs(y - (x - x_reduction));
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kParentT:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.8);
        double y_reduction = std::abs(y - (x - x_reduction));
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kUses:
      // Not too good
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.2);
        double y_reduction = y - (x - x_reduction);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kModifies:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.1);
        double y_reduction = (y * 0.2);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.2;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kCalls:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kCallsT:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kNext:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kNextT:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kAffects:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kAffectsT:
      return [](int x, int y) {
        int nesting_level = 1;
        double nesting_bias = (2 - (1.0 / (sqrt(nesting_level))));
        double x_reduction = (x * 0.05);
        double y_reduction = (y * 0.05);
        double cost = std::max(x - x_reduction, y - y_reduction) * nesting_bias * 2.5;
        return (std::max(x_reduction, y_reduction) * (x_reduction + y_reduction)) / cost;
      };
    case RsType::kAssignPattern:
      // Need assign/stmt ratio
      return [](int x, int y) { return -1; };
    default:
      return [](int x, int y) { return INT16_MAX; };
  }
}