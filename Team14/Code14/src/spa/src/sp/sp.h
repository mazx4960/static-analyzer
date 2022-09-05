#pragma once

#include "pkb/public/pkb.h"

class SP {
 private:
  PKB *pkb_interface_ = nullptr;

 public:
  SP() = default;
  virtual void set_interface(PKB *);

};
