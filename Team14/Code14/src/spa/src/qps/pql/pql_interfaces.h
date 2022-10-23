#pragma once

class ICheckSemantics {
 public:
  [[nodiscard]] virtual bool isSemanticallyCorrect() const = 0;
};
