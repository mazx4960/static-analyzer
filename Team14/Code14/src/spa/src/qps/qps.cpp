// Copyright 2022 CS3203 Team14. All rights reserved.
#include "qps.h"

void QPS::SetPKB(IPKBGetter *pkb) {
  this->pkb_ = pkb;
}
Query QPS::parse(std::ifstream *query) {
  return QueryParser::parse(query);
}
Result QPS::evaluate(Query &query) {
  return (new QueryEvaluator(this->pkb_, query))->evaluate();
}
