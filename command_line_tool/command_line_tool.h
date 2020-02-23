#ifndef COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_
#define COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include <memory>
#include <string>
#include <iostream>
#include <utility>

#include "func/func_client.h"
#include "warble/warble.pb.h"

namespace command_line {

// Handler class for command line interface.
// It is responsible for parsing command line flags and sending appropriate
// requests to func_server.
class CommandLineHandler {
 public:
  // Constructor: takes in a func_client pointer
  // During Run(), func_client_ will be used to communicate to func_server
  explicit CommandLineHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : func_client_(client) {}

  // Disable move and copy
  CommandLineHandler(const CommandLineHandler&) = delete;
  CommandLineHandler& operator=(const CommandLineHandler&) = delete;

  // Initialization: hook all event functions
  bool Init();

  // Parses command line flag and request for message
  bool Run();

 private:
  // Defines a pair with event_type and event_function name
  typedef std::pair<int, std::string> event_pair;

  // -- Flag validation helper functions --

  // Returns true if flags are for REGISTER
  bool ValidRegCmd();

  // Returns true if flags are for NEW_WARBLE
  bool ValidWarbleCmd();

  // Returns true if flags are for FOLLOW
  bool ValidFollowCmd();

  // Returns true if flags are for READ_WARBLE
  bool ValidReadCmd();

  // Returns true if flags are for READ_WARBLE
  bool ValidProfileCmd();

  // Return true if flags are for hook
  bool ValidHookCmd();

  // -- Communication helper functions --

  // Sends registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendRegisteruser();

  // Sends registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendWarble();

  // Sends registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendFollow();

  // Sends registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendRead();

  // Sends registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendProfile();

  // Pointer to Func client that communicates to Func server
  std::shared_ptr<func::FuncClientAbstract> func_client_;

  // Stores event_pairs as constants
  const event_pair kRegisteruserPair = event_pair(1, "registeruser");
  const event_pair kWarblePair = event_pair(2, "warble");
  const event_pair kFollowPair = event_pair(3, "follow");
  const event_pair kReadPair = event_pair(4, "read");
  const event_pair kProfilePair = event_pair(5, "profile");
};

}  // namespace command_line

#endif  // COMMAND_LINE_TOOL_COMMAND_LINE_TOOL_H_
