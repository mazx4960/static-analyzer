#pragma once

#include <utility>
#include "qps/query_evaluator/database/database.h"
#include "qps/result.h"

/**
 * Base projector class.
 */
class ResultProjector {
 protected:
  explicit ResultProjector(Database *database) : database_(database) {
  };
  Database *database_;
 public:
  /**
   * Factory method to create projector.
   * @return result projector
   */
  static ResultProjector *NewProjector(SelectCall *, Database *);
  /**
   * Projects and merges subquery result tables and creates an instance of Result class.
   * @return instance of Result class matching the Select call.
   */
  virtual Result *Project() = 0;
};

class ElemSelectProjector : public ResultProjector {
 private:
  std::vector<ElemReference *> selected_;

 public:
  ElemSelectProjector(std::vector<ElemReference *> &selected, Database *database)
      : selected_(std::move(selected)), ResultProjector(database) {
  };
  Result *Project() override;
};

class BooleanSelectProjector : public ResultProjector {
 public:
  explicit BooleanSelectProjector(Database *database) : ResultProjector(database) {
  };
  Result *Project() override;
};
