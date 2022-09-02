#pragma once

#include "pkb/component_interface/pkb_interface.h"

class QPS {
 private:
  PKBInterface *pkb_interface = nullptr;

 public:
  QPS();
  void set_interface(PKBInterface *);
};