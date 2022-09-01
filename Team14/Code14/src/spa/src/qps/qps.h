#pragma once

#include "pkb/component_interface/pkb_interface.h"
#include "qps/component_interface/qps_interface.h"

class QPS {
 private:
  QPSInterface interface;

  PKBInterface pkb_interface;

 public:
  QPS();
  virtual QPSInterface get_interface();
  void set_interface(PKBInterface);
};