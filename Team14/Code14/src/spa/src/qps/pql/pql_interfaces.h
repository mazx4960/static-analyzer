#pragma once

class ICheckSemantics {
 public:
  [[nodiscard]] virtual bool isSemanticallyCorrect() const = 0;
};

class IWeight {
 public:
  virtual void SetWeight(double weight) = 0;
  [[nodiscard]] virtual double GetWeight() const = 0;
};
