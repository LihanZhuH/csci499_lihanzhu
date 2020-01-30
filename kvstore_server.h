#ifndef KVSTORE_SERVER_H_
#define KVSTORE_SERVER_H_

#include "kvstore.grpc.pb.h"

#include <unordered_map>
#include <mutex>
#include <string>

#include <grpcpp/grpcpp.h>

// Data Storage
class DataBase {
 public:
  DataBase() {}
  ~DataBase() {}

  // Put 'key' and 'value' into storage
  // Return true if correctly stored, false otherwise
  bool PutIntoStorage(const std::string &key, const std::string &value);

  // Return the value corresponding to 'key'
  // 'success' is set to true if found, false otherwise
  const std::string GetFromStorage(const std::string &key, bool *success);

  // Remove key in storage
  // Return true if key exists, false otherwise
  bool RemoveFromStorage(const std::string &key);

 private:
  // Local storage of key-value pairs
  std::unordered_map<std::string, std::string> storage_map_;

  // Mutex for thread-safe
  std::mutex map_mutex_;
};

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

#endif  // KVSTORE_SERVER_H_