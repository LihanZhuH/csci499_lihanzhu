#include <iostream>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "kvstore.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kvstore::KeyValueStore;
using kvstore::PutRequest;
using kvstore::PutReply;
using kvstore::GetReply;
using kvstore::GetRequest;
using kvstore::RemoveRequest;
using kvstore::RemoveReply;

// Client functionalities
class KeyValueStoreClient {
 public:
  KeyValueStoreClient(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}
  
  // Test with simple put and get
  void TestPut(std::string key, std::string value) {
    {
      ClientContext context;
      PutRequest request;
      PutReply reply;
      request.set_key(key);
      request.set_value(value);

      Status status = stub_->put(&context, request, &reply);
      if (status.ok()) {
        std::cout << "Put succeeded" << std::endl;
      } else {
        std::cout << "Put failed" << std::endl;
      }
    }

    // Check using get
    {
      ClientContext context;
      auto stream = stub_->get(&context);

      GetRequest request;
      request.set_key(key);
      stream->Write(request);

      GetReply response;
      stream->Read(&response);

      std::cout << key << " : " << response.value() << "\n";
      if (response.value() == value) {
        std::cout << "Verified" << std::endl;
      } else {
        std::cout << "Failed" << std::endl;
      }
    }
  }

 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

int main(int argc, char const *argv[])
{
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  KeyValueStoreClient client(channel);

  client.TestPut("A", "B");
  return 0;
}
