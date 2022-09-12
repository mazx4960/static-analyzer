#pragma once

#include "pkb/pkb.h"
#include "qps/query_evaluator/query_evaluator.h"
#include "qps/query_parser.h"
#include "query_lexer.h"

class QPS {
 private:
  IPKBQuerier *pkb_;

 public:
  QPS() = default;
  void SetPKB(IPKBQuerier *pkb);
  Result *EvaluateQuery(std::ifstream &query_stream);
};