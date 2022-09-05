#pragma once

#include "pkb/public/pkb_interface.h"

class QPS {
 private:
  PKBInterface *pkb_interface_ = nullptr;

 public:
  QPS() = default;
  void set_interface(PKBInterface *);
};