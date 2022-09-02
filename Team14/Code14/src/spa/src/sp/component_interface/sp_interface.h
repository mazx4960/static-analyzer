// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/component_interface/pkb_interface.h"
#include "sp/sp.h"

class SPInterface : public Interface {
 private:
  SP component;

 public:
  SPInterface();
  void set_interface(PKBInterface *);
};