// kvstore_server.cc

#include "kvstore_server.h"

#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using kvstore::KeyValueStore;
using kvstore::PutRequest;
using kvstore::PutReply;
using kvstore::GetReply;
using kvstore::GetRequest;
using kvstore::RemoveRequest;
using kvstore::RemoveReply;

Status KeyValueStoreImpl::Put(ServerContext* context,
                              const PutRequest* request,
                              PutReply* response) {
  if (db_.PutIntoStorage(request->key(), request->value())) {
    return Status::OK;
  } else {
    return Status::CANCELLED;
  }
}

Status KeyValueStoreImpl::Get(ServerContext* context,
                              ServerReaderWriter<GetReply, GetRequest>* stream) {
  GetRequest request;
  bool success;
  while (stream->Read(&request)) {
    GetReply reply;
    const std::string value = db_.GetFromStorage(request.key(), &success);
    if (success) {
      reply.set_value(value);
      stream->Write(reply);
    } else {
      return Status::CANCELLED;
    }
  }
  return Status::OK;
}

Status KeyValueStoreImpl::Remove(ServerContext* context,
                                 const RemoveRequest* request,
                                 RemoveReply* response) {
  if (db_.RemoveFromStorage(request->key())) {
    return Status::OK;
  } else {
    return Status::CANCELLED;
  }
}

// Run the server listening on port 50001
void RunServer() {
  std::string server_address("0.0.0.0:50001");
  KeyValueStoreImpl service;

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
