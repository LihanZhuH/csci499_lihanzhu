#include <iostream>

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "func.grpc.pb.h"
#include "func.pb.h"

namespace func {

class FuncServiceClient {
 public:
  FuncServiceClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(FuncService::NewStub(channel)) {}

  // Test of hook
  bool TestHook(int event_type, const std::string& event_function) {
    LOG(INFO) << "FuncServerTest: event_type: " << event_type <<
                 ", event_function: " << event_function;
    grpc::ClientContext context;
    func::HookRequest request;
    func::HookReply reply;
    request.set_event_function(event_function);
    request.set_event_type(event_type);

    grpc::Status status = stub_->Hook(&context, request, &reply);
    if (status.ok()) {
      return true;
    }
    return false;
  }

  // Test of unhook
  bool TestUnhook(int event_type) {
    LOG(INFO) << "FuncServerTest: event_type: " << event_type;
    grpc::ClientContext context;
    func::UnhookRequest request;
    func::UnhookReply reply;
    request.set_event_type(event_type);

    grpc::Status status = stub_->Unhook(&context, request, &reply);
    if (status.ok()) {
      return true;
    }
    return false;
  }

  // Test of event
  bool TestEvent(int event_type) {
    LOG(INFO) << "FuncServerTest: event_type: " << event_type;
    grpc::ClientContext context;
    func::EventRequest request;
    func::EventReply reply;

    google::protobuf::Any payload;
    func::UnhookRequest uhrequest;
    uhrequest.set_event_type(0);
    payload.PackFrom(uhrequest);
    request.set_event_type(event_type);
    request.set_allocated_payload(&payload);

    grpc::Status status = stub_->Event(&context, request, &reply);
    if (status.ok()) {
      return true;
    }
    return false;
  }

 private:
  std::unique_ptr<FuncService::Stub> stub_;
};

}  // namespace func

int main(int argc, char const *argv[]) {
  auto channel = grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials());
  func::FuncServiceClient client(channel);

  if (client.TestHook(1, "registeruser")) {
    LOG(INFO) << "FuncServerTest: success";
  } else {
    LOG(INFO) << "FuncServerTest: failed";
    return 1;
  }

  if (client.TestUnhook(1)) {
    LOG(INFO) << "FuncServerTest: success";
    return 0;
  } else {
    LOG(INFO) << "FuncServerTest: failed";
    return 1;
  }

  if (client.TestEvent(1)) {
    LOG(INFO) << "FuncServerTest: success";
  } else {
    LOG(INFO) << "FuncServerTest: failed";
    return 1;
  }

  return 1;
}