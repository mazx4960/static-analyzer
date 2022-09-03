#pragma once

#include "pkb/component_interface/pkb_interface.h"

class QPS {
 private:
  PKBInterface *pkb_interface_ = nullptr;

 public:
  QPS() = default;
  void set_interface(PKBInterface *);
};