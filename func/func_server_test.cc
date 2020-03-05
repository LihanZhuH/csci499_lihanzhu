#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include <iostream>

#include "func/func.grpc.pb.h"
#include "func/func.pb.h"
#include "warble/warble.pb.h"

namespace func {

// Act as a client to Func server
class FuncServiceClient {
 public:
  explicit FuncServiceClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(FuncService::NewStub(channel)) {}

  // Test of hook
  bool TestHook(int event_type, const std::string& event_function) {
    LOG(INFO) << "TestHook: event_type: " << event_type <<
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
    LOG(INFO) << "TestUnhook: event_type: " << event_type;
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
  bool TestEvent(int event_type, const std::string& event_function) {
    // Hook event type
    grpc::ClientContext hook_context;
    func::HookRequest hook_request;
    func::HookReply hook_reply;
    hook_request.set_event_function(event_function);
    hook_request.set_event_type(event_type);
    grpc::Status status = stub_->Hook(&hook_context, hook_request, &hook_reply);
    if (!status.ok()) {
      return false;
    }

    // Send register
    LOG(INFO) << "TestEvent: event_type: " << event_type;
    grpc::ClientContext context;
    func::EventRequest request;
    func::EventReply reply;

    // Copy from
    google::protobuf::Any payload = google::protobuf::Any();
    warble::RegisteruserRequest register_request;
    register_request.set_username("TEST_0");
    payload.PackFrom(register_request);
    request.set_event_type(event_type);
    auto mutable_payload = request.mutable_payload();
    mutable_payload->CopyFrom(payload);
    LOG(INFO) << "TestEvent: done copyfrom payload";

    status = stub_->Event(&context, request, &reply);
    LOG(INFO) << "TestEvent: done event call";
    if (!status.ok()) {
      return false;
    }
    warble::RegisteruserReply register_reply;
    google::protobuf::Any reply_payload = reply.payload();
    LOG(INFO) << "TestEvent: done reply";
    if (!reply_payload.UnpackTo(&register_reply)) {
      return false;
    }

    return true;
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
    LOG(INFO) << "TestHook: success";
  } else {
    LOG(INFO) << "TestHook: failure";
    return 1;
  }

  if (client.TestUnhook(1)) {
    LOG(INFO) << "TestUnhook: success";
  } else {
    LOG(INFO) << "TestUnhook: failure";
    return 1;
  }

  if (client.TestEvent(1, "registeruser")) {
    LOG(INFO) << "TestEvent: success";
  } else {
    LOG(INFO) << "TestEvent: failure";
    return 1;
  }

  return 0;
}
