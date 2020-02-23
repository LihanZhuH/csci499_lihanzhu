#ifndef COMMAND_LINE_TOOL_H_
#define COMMAND_LINE_TOOL_H_

#include <memory>
#include <string>
#include <iostream>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "func/func_client.h"
#include "warble/warble.pb.h"

namespace command_line {

// Handler class for command line interface
class CommandLineHandler {
 public:
  // Constructor that takes in a func_client pointer
  CommandLineHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : func_client_(client) {}

  // Disable move and copy
  CommandLineHandler(const CommandLineHandler&) = delete;
  CommandLineHandler& operator=(const CommandLineHandler&) = delete;

  // Initialization: hook all event functions
  bool Init();

  // Parse command line flag and request for message
  bool Run();

 private:
  // Define a pair with event_type and event_function name
  typedef std::pair<int, std::string> event_pair;

  // -- Flag validation helper functions --

  // Return true if flags are for REGISTER
  bool ValidRegCmd();
  
  // Return true if flags are for NEW_WARBLE
  bool ValidWarbleCmd();

  // Return true if flags are for FOLLOW
  bool ValidFollowCmd();

  // Return true if flags are for READ_WARBLE
  bool ValidReadCmd();

  // Return true if flags are for READ_WARBLE
  bool ValidProfileCmd();

  // Return true if flags are for hook
  bool ValidHookCmd();

  // -- Communication helper functions --

  // Send registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendRegisteruser();

  // Send registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendWarble();

  // Send registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendFollow();

  // Send registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendRead();

  // Send registeruser request to Func server through func_client
  // Success/failure signaled by return value
  bool SendProfile();

  // Pointer to Func client that communicates to Func server
  std::shared_ptr<func::FuncClientAbstract> func_client_;

  // Event pairs
  const event_pair kRegisteruserPair = event_pair(1, "registeruser");
  const event_pair kWarblePair = event_pair(2, "warble");
  const event_pair kFollowPair = event_pair(3, "follow");
  const event_pair kReadPair = event_pair(4, "read");
  const event_pair kProfilePair = event_pair(5, "profile");
};

}  // command_line

#endif  // COMMAND_LINE_TOOL_H_