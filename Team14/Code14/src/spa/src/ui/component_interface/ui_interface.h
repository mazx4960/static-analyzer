// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "qps/component_interface/qps_interface.h"
#include "sp/component_interface/sp_interface.h"
#include "ui/ui.h"

class UIInterface : public Interface {
 private:
  UI component_;

 public:
  void set_interface(QPSInterface *);
  void set_interface(SPInterface *);
};
