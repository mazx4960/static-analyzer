// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui.h"
#include "commons/component_interface/interface.h"
#include "component_interface/ui_interface.h"

UI::UI() {
  this->interface = UIInterface();
}

std::string UI::getSimpleProgramme() { return {}; }
std::string UI::evaluateSimpleProgramme(std::string) { return {}; }
std::string UI::evaluateQuery(std::string) { return {}; }
std::string UI::display(std::string) { return {}; }

UIInterface UI::get_interface() {
  return this->interface;
}

void UI::set_interface(SPInterface sp_interface) {
  this->sp_interface = sp_interface;
}

void UI::set_interface(QPSInterface qps_interface) {
  this->qps_interface = qps_interface;
}
