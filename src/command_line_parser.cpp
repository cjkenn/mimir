#include <string.h>
#include "command_line_parser.h"
#include "parser_result.h"
#include "error.h"

ParserResult CommandLineParser::Parse(const int argc, char **argv) {
  ParserResult result;

  // If we don't have a filename, we just return early and display a warning message
  if (argc < 2) {
    Error err;
    err.SetMessage("Please provide a filename!");
    result.AddError(err);
    return result;
  }

  std::unordered_map<std::string, std::string> options_map;
  options_map["filename"] = argv[1];

  for (int i = 2; i < argc; i++) {
    if (strncmp(argv[i], "-O", 2) == 0) {
      // TODO: Uhh....
      options_map["optimize"] = "true";
    } else {
      Error err;
      std::string message = "Unknown command line option '";
      message += argv[i];
      message += "'.";
      err.SetMessage(message);
      result.AddError(err);
    }
  }

  result.SetOptions(options_map);
  return result;
}
