#include "kvstore/kvstore_server.h"

#include <glog/logging.h>

#include <iostream>
#include <memory>

namespace kvstore {

grpc::Status KeyValueStoreImpl::Put(grpc::ServerContext* context,
                                    const PutRequest* request,
                                    PutReply* response) {
  LOG(INFO) << "kvstore_server - Put: " << request->key() << ", "
            << request->value();
  if (db_.PutIntoStorage(request->key(), request->value())) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status KeyValueStoreImpl::Get(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<GetReply, GetRequest>* stream) {
  GetRequest request;
  stream->Read(&request);
  GetReply reply;
  // Check whether optional is null
  if (auto value_opt = db_.GetFromStorage(request.key())) {
    // Iterate through vector
    for (auto value : *value_opt) {
      reply.set_value(value);
      stream->Write(reply);
      LOG(INFO) << "kvstore_server - Get: " << request.key() << ", " << value;
    }
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status KeyValueStoreImpl::Remove(grpc::ServerContext* context,
                                       const RemoveRequest* request,
                                       RemoveReply* response) {
  if (db_.RemoveFromStorage(request->key())) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

// Run the server listening on port 50001
void RunServer() {
  std::string server_address("0.0.0.0:50001");
  kvstore::KeyValueStoreImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

}  // namespace kvstore

int main(int argc, char const* argv[]) {
  kvstore::RunServer();

  return 0;
}
