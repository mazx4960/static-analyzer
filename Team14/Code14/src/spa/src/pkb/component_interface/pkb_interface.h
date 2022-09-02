// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/pkb.h"

class PKBInterface : public Interface {
 private:
  PKB component;

 public:
  explicit PKBInterface(PKB component);
};
