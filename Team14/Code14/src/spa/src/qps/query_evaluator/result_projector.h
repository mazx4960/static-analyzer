#pragma once

#include <utility>

#include "commons/entity.h"
#include "evaluation_strategy.h"
#include "qps/result.h"
#include "qps/pql/query_reference.h"
#include "qps/pql/query_call.h"
#include "subquery_result.h"

/**
 * Base projector class.
 */
class ResultProjector {
 protected:
  std::vector<SubqueryResult> subquery_results_;

  SubqueryResult joined_results_ = SubqueryResult::FullNoSynonym();

  explicit ResultProjector(std::vector<SubqueryResult> &subquery_results) : subquery_results_(subquery_results) {
  };

  /**
   * Join results from list of subquery results.
   * Joined table is stored in instance attribute joined_results_.
   */
  void join();

  virtual SubqueryResult getEmptyFinalTable() = 0;

 public:
  static ResultProjector *getProjector(SelectCall *, std::vector<SubqueryResult> &);

  /**
   * Projects and merges subquery result tables and creates an instance of Result class.
   * @return instance of Result class matching the Select call.
   */
  virtual Result *getResult() = 0;
};

class ElemSelectProjector : public ResultProjector {
 private:
  std::vector<ElemReference *> called_declarations_;

  std::vector<QuerySynonym *> called_synonyms_;

  SubqueryResult getEmptyFinalTable() override;

  /**
   * Select results (columns) based on called synonyms.
   * @return subset of intermediate result table with only columns for selected synonyms.
   */
  SubqueryResult selectResults();

 public:
  ElemSelectProjector(std::vector<ElemReference *> &declarations, std::vector<SubqueryResult> &subquery_results);

  Result *getResult() override;
};

class BooleanSelectProjector : public ResultProjector {
 private:
  SubqueryResult getEmptyFinalTable() override;

  /**
  * Checks if final table has any rows.
  * @return true if table is non empty.
  */
  bool has_results();

 public:
  explicit BooleanSelectProjector(std::vector<SubqueryResult> &subquery_results) : ResultProjector(subquery_results) {
  };

  Result *getResult() override;
};
