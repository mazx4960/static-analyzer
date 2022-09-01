#pragma once

#include "commons/component_interface/interface.h"
#include "pkb/component_interface/pkb_interface.h"
#include "sp/component_interface/sp_interface.h"
class SP {
 private:
  SPInterface interface;

  PKBInterface pkb_interface;

 public:
  SP();
  virtual SPInterface get_interface();
  virtual void set_interface(PKBInterface);

};
