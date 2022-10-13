class ICheckSemantics{
 public:
  [[nodiscard]] virtual bool IsSemanticallyCorrect() const = 0;
};

