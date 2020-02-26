#ifndef COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_
#define COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <utility>

#include "func/func_client.h"
#include "command_line_tool/command_handler.h"

namespace command_line {

// Handler class for command line user interface.
// It is responsible for parsing command line flags and sending appropriate
// requests to func_server.
class CommandLineTool {
 public:
  // Constructor: takes in a func_client pointer
  // During Run(), func_client_ will be used to communicate to func_server
  explicit CommandLineTool(std::shared_ptr<func::FuncClientAbstract> client)
      : func_client_(client),
        handler_vector_({std::make_shared<RegisterHandler>(client),
                         std::make_shared<WarbleHandler>(client),
                         std::make_shared<FollowHandler>(client),
                         std::make_shared<ReadHandler>(client),
                         std::make_shared<ProfileHandler>(client)}) {}

  // Disable move and copy
  CommandLineTool(const CommandLineTool&) = delete;
  CommandLineTool& operator=(const CommandLineTool&) = delete;

  // Parses command line flag and request for message
  bool Run();

 private:
  // Pointer to Func client that communicates to Func server
  std::shared_ptr<func::FuncClientAbstract> func_client_;

  // A vector of all command handlers
  std::vector<std::shared_ptr<Handler>> handler_vector_;
};

}  // namespace command_line

#endif  // COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_
