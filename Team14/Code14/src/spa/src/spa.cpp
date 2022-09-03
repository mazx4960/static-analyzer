#include "spa.h"

SPA::SPA() {
  /*
   * Pass interfaces to other components with dependency only.
   */
  // QPS only depends on PKB for retrieval.
  this->qps_interface_.set_interface(&this->pkb_interface_);

  // SP only depends on PKB for saving.
  this->sp_interface_.set_interface(&this->pkb_interface_);

  // UI depends on SP and QPS.
  this->ui_interface_.set_interface(&this->sp_interface_);
  this->ui_interface_.set_interface(&this->qps_interface_);
}