// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "sp/component_interface/sp_interface.h"
#include "pkb/component_interface/pkb_interface.h"

class PKB {
 private:
  PKBInterface interface;

 public:
  PKB();
  virtual PKBInterface get_interface();
};
