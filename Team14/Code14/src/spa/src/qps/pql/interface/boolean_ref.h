class IBooleanRef {
 public:
  [[nodiscard]] virtual bool isBooleanRef() const = 0;
  virtual void setBooleanRef(bool is_boolean_Ref) = 0;
};
