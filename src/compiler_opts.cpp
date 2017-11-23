#include <string.h>
#include "compiler_opts.h"
#include "error.h"

using namespace mimir;

const std::string FILENAME_KEY = "filename";
const std::string LOCAL_OPT_KEY = "local_opt";
const std::string AST_PRINT_KEY = "ast_print";
const std::string IR_PRINT_KEY = "ir_print";

const char* LOCAL_OPT = "-O";
const char* AST_PRINT_OPT = "-a";
const char* IR_PRINT_OPT = "-i";

void CompilerOptions::ParseCommandLine(const int argc, char **argv) {
  // If we don't have a filename, we just return early and display a warning message
  if (argc < 2) {
    Error err;
    err.SetMessage("Please provide a filename!");
    errors_.push_back(err);
    return;
  }

  options_map_[FILENAME_KEY] = argv[1];

  for (int i = 2; i < argc; i++) {
    if (strncmp(argv[i], LOCAL_OPT, 2) == 0) {
      options_map_[LOCAL_OPT_KEY] = LOCAL_OPT;
    } else if (strncmp(argv[i], AST_PRINT_OPT, 2) == 0) {
      options_map_[AST_PRINT_KEY] = AST_PRINT_OPT;
    } else if (strncmp(argv[i], IR_PRINT_OPT, 2) == 0) {
      options_map_[IR_PRINT_KEY] = IR_PRINT_OPT;
    } else {
      errors_.push_back(BuildUnknownOptionError(argv[i]));
    }
  }
}

bool CompilerOptions::HasCommandLineErrors() {
  return errors_.size() > 0;
}

void CompilerOptions::ReportCommandLineErrors() {
  for (auto err : errors_) {
    err.Report();
  }
}

std::string CompilerOptions::GetInputFilename() {
  return options_map_[FILENAME_KEY];
}

bool CompilerOptions::ShouldPerformLocalOptimization() {
  return (options_map_.find(LOCAL_OPT_KEY) != options_map_.end());
}

bool CompilerOptions::ShouldPrintAST() {
  return (options_map_.find(AST_PRINT_KEY) != options_map_.end());
}

bool CompilerOptions::ShouldPrintIR() {
  return (options_map_.find(IR_PRINT_KEY) != options_map_.end());
}

Error CompilerOptions::BuildUnknownOptionError(const std::string unknown_opt) {
  Error err;
  std::string message = "Unknown command line option '";
  message += unknown_opt;
  message += "'.";
  err.SetMessage(message);

  return err;
}
