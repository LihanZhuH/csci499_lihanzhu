#ifndef COMMAND_LINE_TOOL_COMMAND_HANDLER_H_
#define COMMAND_LINE_TOOL_COMMAND_HANDLER_H_

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include <memory>
#include <string>
#include <iostream>

#include "func/func_client.h"
#include "warble/warble.pb.h"

namespace command_line {

// An abstract handler class for a command
// Can be inherited to process different commands
// Must call IsValidCommand() before ProcessCommand()
class Handler {
 public:
  // Default constructor
  explicit Handler(std::shared_ptr<func::FuncClientAbstract> client)
      : func_client_(client) {}

  // Default destructor
  virtual ~Handler() {}

  // Returns true if this handler can process this command
  // Uses gflags to validate
  virtual bool IsValidCommand() = 0;

  // Processes this command using inputs from gflag
  // Returns bool to represent success/failure
  virtual bool ProcessCommand() = 0;

 protected:
  // Hook event function with event type
  bool HookEventFunction(int event_type, std::string event_function);

  // Pointer to Func client that communicates to Func server
  std::shared_ptr<func::FuncClientAbstract> func_client_;
};

// The handler class for registeruser command
class RegisterHandler : public Handler {
 public:
  // Default constructor
  explicit RegisterHandler(std::shared_ptr<func::FuncClientAbstract> client)
      :Handler(client) {}

  // Default destructor
  ~RegisterHandler() {}

  // See base class
  bool IsValidCommand() override;

  // See base class
  bool ProcessCommand() override;

 private:
  // Constant event type and event function
  const int kRegisterEventType = 1;
  const std::string kRegisterEventFunction = "registeruser";
};

// The handler class for warble command
class WarbleHandler : public Handler {
 public:
  // Default constructor
  explicit WarbleHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : Handler(client) {}

  // Default destructor
  ~WarbleHandler() {}

  // See base class
  bool IsValidCommand() override;

  // See base class
  bool ProcessCommand() override;

 private:
  // Constant event type and event function
  const int kWarbleEventType = 2;
  const std::string kWarbleEventFunction = "warble";
};

// The handler class for follow command
class FollowHandler : public Handler {
 public:
  // Default constructor
  explicit FollowHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : Handler(client) {}

  // Default destructor
  ~FollowHandler() {}

  // See base class
  bool IsValidCommand() override;

  // See base class
  bool ProcessCommand() override;

 private:
  // Constant event type and event function
  const int kFollowEventType = 3;
  const std::string kFollowEventFunction = "follow";
};

// The handler class for read command
class ReadHandler : public Handler {
 public:
  // Default constructor
  explicit ReadHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : Handler(client) {}

  // Default destructor
  ~ReadHandler() {}

  // See base class
  bool IsValidCommand() override;

  // See base class
  bool ProcessCommand() override;

 private:
  // Constant event type and event function
  const int kReadEventType = 4;
  const std::string kReadEventFunction = "read";
};

// The handler class for profile command
class ProfileHandler : public Handler {
 public:
  // Default constructor
  explicit ProfileHandler(std::shared_ptr<func::FuncClientAbstract> client)
      : Handler(client) {}

  // Default destructor
  ~ProfileHandler() {}

  // See base class
  bool IsValidCommand() override;

  // See base class
  bool ProcessCommand() override;

 private:
  // Constant event type and event function
  const int kProfileEventType = 5;
  const std::string kProfileEventFunction = "profile";
};

}  // namespace command_line

#endif  // COMMAND_LINE_TOOL_COMMAND_HANDLER_H_
