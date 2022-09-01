#include "sp.h"

SP::SP() {
  this->interface = SPInterface();
}
SPInterface SP::get_interface() {
  return this->interface;
}
void SP::set_interface(PKBInterface pkb_interface) {
  this->pkb_interface = pkb_interface;
}
