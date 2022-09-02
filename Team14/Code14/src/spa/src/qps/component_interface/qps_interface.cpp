// Copyright 2022 CS3203 Team14. All rights reserved.

#include "qps_interface.h"

QPSInterface::QPSInterface(QPS *component) {
  this->component = component;
}
void QPSInterface::set_interface(PKBInterface *interface) {
  this->component->set_interface(interface);
}
