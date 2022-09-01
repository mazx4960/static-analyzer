#include "spa.h"

SPA::SPA() {
  /*
   * Instantiate subcomponents.
   */
  this->pkb = PKB();
  this->qps = QPS();
  this->sp = SP();
  this->ui = UI();

  /*
   * Retrieve subcomponent adaptor interfaces.
   */
  this->pkb_interface = this->pkb.get_interface();
  this->qps_interface = this->qps.get_interface();
  this->sp_interface = this->sp.get_interface();
  this->ui_interface = this->ui.get_interface();

  /*
   * Pass interfaces to other components with dependency only.
   */
  // SP only depends on PKB for saving.
  this->sp.set_interface(this->pkb_interface);

  // QPS only depends on PKB for retrieval.
  this->qps.set_interface(this->pkb_interface);

  // UI depends on SP and QPS.
  this->ui.set_interface(this->sp_interface);
  this->ui.set_interface(this->qps_interface);
}