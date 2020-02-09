#ifndef KVSTORE_SERVER_H_
#define KVSTORE_SERVER_H_

#include "kvstore.grpc.pb.h"

#include <string>

#include <grpcpp/grpcpp.h>

#include "database.h"

namespace kvstore {

// KeyValueStore services
class KeyValueStoreImpl final : public kvstore::KeyValueStore::Service {
 public:
  // Put service
  grpc::Status Put(grpc::ServerContext* context,
                   const kvstore::PutRequest* request,
                   kvstore::PutReply* response) override;

  // Get service
  grpc::Status Get(grpc::ServerContext* context,
                   grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest>* stream)
                   override;

  // Remove service
  grpc::Status Remove(grpc::ServerContext* context,
                      const kvstore::RemoveRequest* request,
                      kvstore::RemoveReply* response) override;

 private:
  DataBase db_;
};

}  // namespace kvstore

#endif  // KVSTORE_SERVER_H_