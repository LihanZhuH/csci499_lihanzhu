#include "func/func_server.h"

#include <glog/logging.h>

namespace func {

grpc::Status FuncServiceImpl::Hook(grpc::ServerContext* context,
                                   const HookRequest* request,
                                   HookReply* response) {
  int event_type = request->event_type();
  const std::string event_function = request->event_function();
  // Call event handler
  if (event_handler_.Hook(event_type, event_function)) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status FuncServiceImpl::Unhook(grpc::ServerContext* context,
                                     const UnhookRequest* request,
                                     UnhookReply* response) {
  int event_type = request->event_type();
  // Call event handler
  if (event_handler_.Unhook(event_type)) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status FuncServiceImpl::Event(grpc::ServerContext* context,
                                    const EventRequest* request,
                                    EventReply* response) {
  int event_type = request->event_type();
  google::protobuf::Any payload = request->payload();
  // Call event handler
  if (auto optional_payload = event_handler_.Event(event_type, payload)) {
    google::protobuf::Any* reply_payload =
        new google::protobuf::Any(*optional_payload);
    response->set_allocated_payload(reply_payload);
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

// Run the server listening on port 50000
void RunServer() {
  std::string server_address("0.0.0.0:50000");
  auto kvstore_channel =
      grpc::CreateChannel("0.0.0.0:50001", grpc::InsecureChannelCredentials());
  func::FuncServiceImpl service(kvstore_channel);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

}  // namespace func

int main(int argc, char const* argv[]) {
  func::RunServer();

  return 0;
}
