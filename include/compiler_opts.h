#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "error.h"

namespace mimir {
class CompilerOptions {
 public:
  CompilerOptions() {};
  ~CompilerOptions() {};

  void ParseCommandLine(const int argc, char **argv);
  bool HasCommandLineErrors();
  void ReportCommandLineErrors();
  std::string GetInputFilename();
  bool ShouldPerformLocalOptimization();

 private:
  Error BuildUnknownOptionError(const std::string unknown_opt);
  std::vector<Error> errors_;
  std::unordered_map<std::string, std::string> options_map_;
};
}
