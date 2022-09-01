// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include "sp/component_interface/sp_interface.h"
#include "qps/component_interface/qps_interface.h"
#include "ui/component_interface/ui_interface.h"

class UI {
 private:
  UIInterface interface;

  SPInterface sp_interface;

  QPSInterface qps_interface;

 public:
  UI();
  std::string getSimpleProgramme();
  std::string evaluateSimpleProgramme(std::string);
  std::string evaluateQuery(std::string);
  std::string display(std::string);

  virtual UIInterface get_interface();
  void set_interface(SPInterface);
  void set_interface(QPSInterface);
};
