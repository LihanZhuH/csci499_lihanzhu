#ifndef KVSTORE_KVSTORE_SERVER_H_
#define KVSTORE_KVSTORE_SERVER_H_

#include <grpcpp/grpcpp.h>

#include <string>

#include "kvstore/kvstore.grpc.pb.h"
#include "kvstore/database.h"

namespace kvstore {

// Implementation of Key Value Store service
// Contains a local database
// Success/failture signaled via grpc::Status
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
  // Thread-safe database
  DataBase db_;
};

}  // namespace kvstore

#endif  // KVSTORE_KVSTORE_SERVER_H_
