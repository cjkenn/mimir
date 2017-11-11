#include "parser_result.h"
#include "error.h"

using namespace mimir;

bool ParserResult::HasError() {
  return errors_.size() > 0;
}

void ParserResult::AddError(Error error) {
  errors_.push_back(error);
}

void ParserResult::ReportErrors() {
  for (auto error : errors_) {
    error.Report();
  }
}
