#ifndef FUNC_CLIENT_H_
#define FUNC_CLIENT_H_

#include <string>
#include <optional>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "func/func.grpc.pb.h"
#include "func/func.pb.h"

namespace func {

// The abstract base class for Func client
// Child classes either have to communicate directly to Func server
// or implemente a local functions for testing
class FuncClientAbstract {
 public:
  FuncClientAbstract() {}
  virtual ~FuncClientAbstract() {}

  // Hook an event type with an event function
  // Success/failure signaled via returned bool
  virtual bool Hook(int event_type, const std::string& event_function) = 0;

  // Unhook an event type
  // Success/failure signaled via returned bool
  virtual bool Unhook(int event_type) = 0;
  
  // Execute the function with the event type and payload
  // Return the reply payload
  virtual std::optional<google::protobuf::Any> Event(int event_type,
      const google::protobuf::Any& payload) = 0;
};

// Func client that communicates with gRPC server
// Provide Hook, Unhook and Event functionalities
class FuncClientImpl : public FuncClientAbstract {
 public:
  // Constructor: require a gRPC channel to connect to
  FuncClientImpl(std::shared_ptr<grpc::Channel> channel)
      : stub_(FuncService::NewStub(channel)) {}

  // Disable move and copy
  FuncClientImpl(const FuncClientImpl&) = delete;
  FuncClientImpl& operator=(const FuncClientImpl&) = delete;
  
  // Hook an event type with an event function
  // Success/failure signaled via returned bool
  bool Hook(int event_type, const std::string& event_function) override;

  // Unhook an event type
  // Success/failure signaled via returned bool
  bool Unhook(int event_type) override;

  // Execute the function with the event type and payload
  // Return the reply payload
  std::optional<google::protobuf::Any> Event(int event_type,
      const google::protobuf::Any& payload) override;

 private:
  // Pointer of stub for connection to func server
  std::unique_ptr<FuncService::Stub> stub_;
};

}  // namespace func

#endif