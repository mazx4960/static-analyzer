#include "spa.h"

SPA::SPA() {
  /*
   * Instantiate subcomponent adaptor interfaces.
   */
  this->pkb_interface = new PKBInterface(PKB());
  this->qps_interface = new QPSInterface(QPS());
  this->sp_interface = new SPInterface(SP());
  this->ui_interface = new UIInterface(UI());

  /*
   * Pass interfaces to other components with dependency only.
   */
  // QPS only depends on PKB for retrieval.
  this->qps_interface->set_interface(this->pkb_interface);

  // SP only depends on PKB for saving.
  this->sp_interface->set_interface(this->pkb_interface);

  // UI depends on SP and QPS.
  this->ui_interface->set_interface(this->sp_interface);
  this->ui_interface->set_interface(this->qps_interface);
}