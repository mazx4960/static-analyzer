// Copyright 2022 CS3203 Team14. All rights reserved.
#include "qps.h"
#include "qps/component_interface/qps_interface.h"

QPS::QPS() {
  this->interface = QPSInterface();
}

QPSInterface QPS::get_interface() {
  return this->interface;
}

void QPS::set_interface(PKBInterface pkb_interface) {
  this->pkb_interface = pkb_interface;
}
