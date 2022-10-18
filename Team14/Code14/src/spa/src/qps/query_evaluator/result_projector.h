#pragma once

#include <utility>

#include "evaluation_strategy.h"
#include "commons/entity.h"
#include "qps/result.h"
#include "qps/pql/query_reference.h"
#include "subquery_result.h"

/**
 * Base projector class.
 */
class ResultProjector {
 protected:
  std::vector<SubqueryResult> &subquery_results_;

  SubqueryResult joined_results_ = SubqueryResult::FullNoSynonym();

  explicit ResultProjector(std::vector<SubqueryResult> &subquery_results) : subquery_results_(subquery_results) {
  };

  /**
   * Join results from list of subquery results.
   */
  void join();

 public:
  /**
   * Project results from list of subquery results.
   */
  virtual void project() = 0;
};

class SelectProjector : public ResultProjector {
 private:
  std::vector<ElemReference *> called_declarations_;

  std::vector<QuerySynonym *> called_synonyms_;

 public:
  SelectProjector(std::vector<ElemReference *> &declarations, std::vector<SubqueryResult> &subquery_results);

  void project() override;

  /**
   * Select results (columns) based on called synonyms.
   * @return subset of intermediate result table with only columns for selected synonyms.
   */
  SubqueryResult select_results();
};

class BooleanProjector : public ResultProjector {
 public:
  explicit BooleanProjector(std::vector<SubqueryResult> &subquery_results) : ResultProjector(subquery_results) {
  };

  void project() override;

  /**
   * Checks if final table has any rows.
   * @return true if table is non empty.
   */
  bool has_results();
};
