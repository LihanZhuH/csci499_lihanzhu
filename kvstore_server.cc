// kvstore_server.cc

#include "kvstore.grpc.pb.h"

#include <unordered_map>
#include <string>

#include <grpcpp/grpcpp.h>

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

// Local storage of key-value pairs
std::unordered_map<std::string, std::string> storage_map;

// Put key and value into storage
// Return true if correctly stored, false otherwise
bool PutIntoStorage(std::string key, std::string value) {
  if (storage_map.count(key) > 0) {
    return false;
  } else {
    storage_map.insert({key, value});
    return true;
  }
}

// Return value corresponding to key
// Return an empty string if not found
std::string GetFromStorage(std::string key) {
  if (storage_map.count(key) > 0) {
    return storage_map[key];
  } else {
    return "";
  }
}

// Remove key in storage
// Return true if key exists, false otherwise
bool RemoveFromStorage(std::string key) {
  if (storage_map.count(key) > 0) {
    storage_map.erase(key);
    return true;
  } else {
    return false;
  }
}

// KeyValueStore services
class KeyValueStoreImpl final : public KeyValueStore::Service {
  Status put(ServerContext* context, const PutRequest* request,
             PutReply* response) override {
    if (PutIntoStorage(request->key(), request->value())) {
      return Status::OK;
    } else {
      return Status::CANCELLED;
    }
  }

  Status get(ServerContext* context,
             ServerReaderWriter<GetReply, GetRequest>* stream) override {
    GetRequest request;
    while (stream->Read(&request)) {
      GetReply reply;
      reply.set_value(GetFromStorage(request.key()));
      stream->Write(reply);
    }
    return Status::OK;
  }

  Status remove(ServerContext* context, const RemoveRequest* request,
                RemoveReply* response) override {
    if (RemoveFromStorage(request->key())) {
      return Status::OK;
    } else {
      return Status::CANCELLED;
    }
  }

};

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

int main(int argc, char const *argv[])
{
  RunServer();

  return 0;
}
