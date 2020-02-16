#include "func_server.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include "func.grpc.pb.h"

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
  
  // TODO

  return grpc::Status::OK;
}

// Run the server listening on port 50000
void RunServer() {
  std::string server_address("0.0.0.0:50000");
  func::FuncServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

}  // namespace func

int main(int argc, char const *argv[]) {
  func::RunServer();

  return 0;
}