// Copyright 2022 CS3203 Team14. All rights reserved.

#include "sp_interface.h"

SPInterface::SPInterface(SP *component) {
  this->component = component;
}
void SPInterface::set_interface(PKBInterface *interface) {
  this->component->set_interface(interface);
}
