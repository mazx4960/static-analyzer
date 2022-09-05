// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/public/pkb.h"
#include "sp/sp.h"

class SPInterface : public Interface {
 private:
  SP component_;

 public:
  void set_interface(PKB *);
};