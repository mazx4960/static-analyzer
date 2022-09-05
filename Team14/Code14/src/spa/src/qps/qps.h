#pragma once

#include "pkb/component_interface/pkb_interface.h"

class QPS {
 private:
  PKB* pkb_;

 public:
  QPS() = default;
  void SetPKB(PKB* pkb);
};