// kvstore_server.cc

#include <grpcpp/grpcpp.h>

#include "kvstore.grpc.pb.h"

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

// KeyValueStore services
class KeyValueStoreImpl final : public KeyValueStore::Service {
  Status put(ServerContext* context, const PutRequest* request,
             PutReply* response) override {

  }

  Status get(ServerContext* context,
             ServerReaderWriter<GetReply, GetRequest>* stream) override {

  }

  Status remove(ServerContext* context, const RemoveRequest* request,
                RemoveReply* response) override {

  }

 private:
  
};

int main(int argc, char const *argv[])
{
  
  return 0;
}
