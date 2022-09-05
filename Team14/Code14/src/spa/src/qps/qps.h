#pragma once

#include "pkb/pkb.h"
#include "qps/query_parser/query_lexer.h"
#include "qps/query_evaluator/query_evaluator.h"

class QPS {
 private:
  PKB *pkb_;

  QueryLexer *query_lexer_;

  QueryEvaluator *query_evaluator_;

 public:
  QPS() = default;
  void SetPKB(PKB *pkb);
};