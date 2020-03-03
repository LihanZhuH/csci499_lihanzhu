#include "command_line_tool/command_line_tool.h"

namespace command_line {

bool CommandLineTool::Run() {
  // Loop through all handlers
  for (auto handler : handler_vector_) {
    if (handler->IsValidCommand()) {
      return handler->ProcessCommand();
    }
  }
  std::cout << "Invalid Command!" << std::endl;
  return false;
}

}  // namespace command_line
