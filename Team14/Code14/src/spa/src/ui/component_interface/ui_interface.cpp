// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui_interface.h"

UIInterface::UIInterface() {
  this->component = UI();
}
void UIInterface::set_interface(QPSInterface *interface) {
  this->component.set_interface(interface);
}
void UIInterface::set_interface(SPInterface *interface) {
  this->component.set_interface(interface);
}
