#include "func/func_client.h"

namespace func {

bool FuncClientImpl::Hook(int event_type, const std::string& event_function) {
  // Set up request
  grpc::ClientContext context;
  func::HookRequest request;
  func::HookReply reply;
  request.set_event_function(event_function);
  request.set_event_type(event_type);
  // Call Func server function
  auto status = stub_->Hook(&context, request, &reply);
  if (!status.ok()) {
    LOG(WARNING) << "Func Client: Hook failed";
    return false;
  }
  return true;
}

bool FuncClientImpl::Unhook(int event_type) {
  // Set up request
  grpc::ClientContext context;
  func::UnhookRequest request;
  func::UnhookReply reply;
  request.set_event_type(event_type);
  // Call Func server function
  auto status = stub_->Unhook(&context, request, &reply);
  if (!status.ok()) {
    LOG(WARNING) << "Func Client: Unhook failed";
    return false;
  }
  return true;
}

std::optional<google::protobuf::Any> FuncClientImpl::Event(int event_type,
    const google::protobuf::Any& payload) {
  // Set up request
  grpc::ClientContext context;
  func::EventRequest request;
  func::EventReply reply;

  google::protobuf::Any* mutable_payload = request.mutable_payload();
  mutable_payload->CopyFrom(payload);
  request.set_event_type(event_type);
  // Call Func server function
  auto status = stub_->Event(&context, request, &reply);
  if (!status.ok()) {
    LOG(WARNING) << "Func Client: Event failed";
    return std::nullopt;
  }
  return reply.payload();
}

}  // namespace func
