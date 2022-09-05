#pragma once

#include "pkb/public/pkb.h"

class QPS {
 private:
  PKB *pkb_interface_ = nullptr;

 public:
  QPS() = default;
  void set_interface(PKB *);
};