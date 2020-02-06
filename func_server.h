#ifndef FUNC_SERVER_H_
#define FUNC_SERVER_H_

#include "func.grpc.pb.h"

#include <unordered_map>
#include <string>

#include <grpcpp/grpcpp.h>

// Implementation of Func Service
class FuncServiceImpl final : func::FuncService::Service {
 public:
  // Hook an event type with an event function
  grpc::Status Hook(grpc::ServerContext* context,
                    const func::HookRequest* request,
                    func::HookReply* response);

  // Unhook an event type with an event function
  grpc::Status Unhook(grpc::ServerContext* context,
                      const func::UnhookRequest* request,
                      func::UnhookReply* response);

  // Process an imcoming request and send a response
  grpc::Status Event(grpc::ServerContext* context,
                    const func::EventRequest* request,
                    func::EventReply* response);

 private:
  std::unordered_map<int, std::string> event_map_;
};

#endif  // FUNC_SERVER_H_