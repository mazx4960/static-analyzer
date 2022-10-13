#include <string>
#include <utility>

class QueryAttribute {
 private:
  std::string attribute_name_;
 protected:
  explicit QueryAttribute(std::string attribute_name) : attribute_name_(std::move(attribute_name)){}
 public:
  bool operator==(const QueryAttribute &other) const;
  [[nodiscard]] std::string toString() const;

};

class ProcAttribute: public QueryAttribute{
 public:
  explicit ProcAttribute() : QueryAttribute("procName"){}
};

class VarAttribute: public QueryAttribute{
 public:
  explicit VarAttribute() : QueryAttribute("varName"){}
};

class ValueAttribute: public QueryAttribute{
 public:
  explicit ValueAttribute() : QueryAttribute("value"){}
};

class StmtAttribute: public QueryAttribute{
 public:
  explicit StmtAttribute() : QueryAttribute("stmt#"){}
};