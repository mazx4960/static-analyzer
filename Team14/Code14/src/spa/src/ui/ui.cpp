// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui.h"

std::string UI::getSimpleProgramme() { return {}; }
std::string UI::evaluateSimpleProgramme(std::string) { return {}; }
std::string UI::evaluateQuery(std::string) { return {}; }
std::string UI::display(std::string) { return {}; }

void UI::set_interface(QPSInterface *qps_interface) {
  this->qps_interface_ = qps_interface;
}

void UI::set_interface(SPInterface *sp_interface) {
  this->sp_interface_ = sp_interface;
}
