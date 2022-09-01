// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb.h"
#include "pkb/component_interface/pkb_interface.h"
PKB::PKB() {
  this->interface = PKBInterface();
}
PKBInterface PKB::get_interface() {
  return this->interface;
}
