#pragma once

#include "pkb/pkb.h"
#include "qps/query_evaluator/query_evaluator.h"
#include "qps/query_parser/query_lexer.h"
#include "qps/query_parser/query_parser.h"

class QPS {
 private:
  IPKBQuerier *pkb_;

 public:
  QPS() = default;
  void SetPKB(IPKBQuerier *pkb);
  static Query parse(std::ifstream *);
  Result evaluate(Query &query);
};