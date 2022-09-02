// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include "sp/component_interface/sp_interface.h"
#include "qps/component_interface/qps_interface.h"

class UI {
 private:
  SPInterface *sp_interface = nullptr;

  QPSInterface *qps_interface = nullptr;

 public:
  UI() = default;
  std::string getSimpleProgramme();
  std::string evaluateSimpleProgramme(std::string);
  std::string evaluateQuery(std::string);
  std::string display(std::string);

  void set_interface(QPSInterface *);
  void set_interface(SPInterface *);
};
