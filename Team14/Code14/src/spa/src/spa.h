// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/pkb.h"
#include "pkb/component_interface/pkb_interface.h"
#include "qps/qps.h"
#include "qps/component_interface/qps_interface.h"
#include "sp/sp.h"
#include "sp/component_interface/sp_interface.h"
#include "ui/ui.h"
#include "ui/component_interface/ui_interface.h"

class SPA {
 private:
  PKBInterface *pkb_interface;

  QPSInterface *qps_interface;

  SPInterface *sp_interface;

  UIInterface *ui_interface;

 public:
  SPA();
};
