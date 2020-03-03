#ifndef FUNC_FUNC_SERVER_H_
#define FUNC_FUNC_SERVER_H_

#include <grpcpp/grpcpp.h>
#include <google/protobuf/any.pb.h>

#include <unordered_map>
#include <string>
#include <set>
#include <memory>

#include "func/event_handler.h"
#include "kvstore/kvstore_client.h"
#include "func/func.grpc.pb.h"

namespace func {

// Implementation of Func Service
class FuncServiceImpl final : public func::FuncService::Service {
 public:
  // Constructor
  explicit FuncServiceImpl(std::shared_ptr<grpc::Channel> channel)
       : event_handler_(std::make_shared<kvstore::KVStoreWarbleClient>
                        (channel)) {}

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
  // Handler to process hook, unhook and event requests
  EventHandler event_handler_;
};

}  // namespace func

#endif  // FUNC_FUNC_SERVER_H_
