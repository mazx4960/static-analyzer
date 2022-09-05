#pragma once

#include <fstream>

#include "pkb/component_interface/pkb_interface.h"

class SP {
 private:
  PKB* pkb_;

 public:
  SP() = default;
  void SetPKB(PKB* pkb);
  void LoadSource(std::ifstream& source_stream);
};
