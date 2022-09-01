#include "spa.h"

SPA::SPA() {
  this->pkb = PKB();
  this->qps = QPS();
  this->sp = SP();
  this->ui = UI();

  this->pkb_interface = this->pkb.get_interface();
  this->qps_interface = this->qps.get_interface();
  this->sp_interface = this->sp.get_interface();
  this->ui_interface = this->ui.get_interface();

  this->sp.set_interface(this->pkb_interface);

  this->qps.set_interface(this->pkb_interface);

  this->ui.set_interface(this->sp_interface);
  this->ui.set_interface(this->qps_interface);
}