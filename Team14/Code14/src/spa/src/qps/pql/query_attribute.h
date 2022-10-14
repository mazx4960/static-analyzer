#include <string>
#include <utility>
enum class AttributeType {
  kProcName,
  kVarName,
  kValue,
  kStmtNo
};
class QueryAttribute {
 private:
  AttributeType attribute_type_;
 protected:
  explicit QueryAttribute(AttributeType attribute_type) : attribute_type_(attribute_type){}
 public:
  bool operator==(const QueryAttribute &other) const;
  [[nodiscard]] std::string toString() const;

};

class ProcAttribute: public QueryAttribute{
 public:
  explicit ProcAttribute() : QueryAttribute(AttributeType::kProcName){}
};

class VarAttribute: public QueryAttribute{
 public:
  explicit VarAttribute() : QueryAttribute(AttributeType::kVarName){}
};

class ValueAttribute: public QueryAttribute{
 public:
  explicit ValueAttribute() : QueryAttribute(AttributeType::kValue){}
};

class StmtAttribute: public QueryAttribute{
 public:
  explicit StmtAttribute() : QueryAttribute(AttributeType::kStmtNo){}
};