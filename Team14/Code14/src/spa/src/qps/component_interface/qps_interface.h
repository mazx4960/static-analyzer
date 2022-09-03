// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/component_interface/pkb_interface.h"
#include "qps/qps.h"

class QPSInterface : public Interface {
 private:
  QPS component_;

 public:
  void set_interface(PKBInterface *);
};