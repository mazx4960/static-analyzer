#pragma once

#include "pkb/public/pkb.h"

class QPS {
 private:
  PKB *pkb_;

 public:
  QPS() = default;
  void SetPKB(PKB *pkb);
};