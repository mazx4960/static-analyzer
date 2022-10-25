#pragma once

class IAttrCompareRef {
 public:
  [[nodiscard]] virtual bool isAttrCompareRef() const = 0;
};

class IBooleanRef {
 public:
  [[nodiscard]] virtual bool isBooleanRef() const = 0;
  virtual void setBooleanRef(bool is_boolean_Ref) = 0;
};

class ICheckSemantics {
 public:
  [[nodiscard]] virtual bool isSemanticallyCorrect() const = 0;
};

class ICheckSyntax {
 public:
  [[nodiscard]] virtual bool isSyntacticallyCorrect() const = 0;
};

class IEntRef {
 public:
  [[nodiscard]] virtual bool isEntRef() const = 0;
};

class IElem {
 public:
  [[nodiscard]] virtual bool isElem() const = 0;
};

class IStmtRef {
 public:
  [[nodiscard]] virtual bool isStmtRef() const = 0;
};
