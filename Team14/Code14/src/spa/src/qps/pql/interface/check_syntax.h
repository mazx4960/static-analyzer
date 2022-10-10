class ICheckSyntax {
 public:
  [[nodiscard]] virtual bool isSyntacticallyCorrect() const = 0;
};

