#pragma once

#include "pkb/component_interface/pkb_interface.h"

class SP {
 private:
  PKBInterface *pkb_interface_ = nullptr;

 public:
  SP() = default;
  virtual void set_interface(PKBInterface *);

};