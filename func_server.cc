#include "func_server.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include "func.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using func::HookRequest;
using func::HookReply;
using func::UnhookRequest;
using func::UnhookReply;
using func::EventRequest;
using func::EventReply;

namespace func {

Status FuncServiceImpl::Hook(ServerContext* context,
                             const HookRequest* request,
                             HookReply* response) {
  int event_type = request->event_type();
  const std::string event_function = request->event_function();
  // Call event handler
  if (event_handler_.Hook(event_type, event_function)) {
    return Status::CANCELLED;
  }
  return Status::OK;
}

Status FuncServiceImpl::Unhook(ServerContext* context,
                               const UnhookRequest* request,
                               UnhookReply* response) {
  int event_type = request->event_type();
  // Call event handler
  if (event_handler_.Unhook(event_type)) {
    return Status::CANCELLED;
  }
  return Status::OK;
}

Status FuncServiceImpl::Event(ServerContext* context,
                              const EventRequest* request,
                              EventReply* response) {
  int event_type = request->event_type();
  
  // TODO

  return Status::OK;
}

}  // namespace func

// Run the server listening on port 50000
void RunServer() {
  std::string server_address("0.0.0.0:50000");
  func::FuncServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char const *argv[]) {
  RunServer();

  return 0;
}