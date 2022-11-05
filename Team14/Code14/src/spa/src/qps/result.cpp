#include <spdlog/fmt/bundled/format.h>
#include "result.h"

Result *Result::Empty() {
  return new Result({}, {});
}

Result *Result::True() {
  return new Result({}, {Result::kBooleanTrue});
}

Result *Result::False() {
  return new Result({}, {Result::kBooleanFalse});
}

std::unordered_set<std::string> Result::GetResults() const {
  return this->results_;
}

std::string Result::ToString() const {
  std::string result_string;
  result_string += "Selected: <";
  for (auto *elem_ref : this->selected_) {
    result_string += " " + elem_ref->toString();
  }
  result_string += " >\n";
  result_string += "Results[" + std::to_string(this->results_.size()) + "]: ";
  for (const auto &result : this->results_) {
    result_string += result + ", ";
  }
  result_string += "\n";
  return result_string;
}
