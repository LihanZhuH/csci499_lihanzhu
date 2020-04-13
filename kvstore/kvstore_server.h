#ifndef KVSTORE_KVSTORE_SERVER_H_
#define KVSTORE_KVSTORE_SERVER_H_

#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>

#include <fstream>
#include <string>

#include "kvstore/database.h"
#include "kvstore/kvstore.grpc.pb.h"

namespace kvstore {

// Implementation of Key Value Store service
// Contains a local database
// Success/failture signaled via grpc::Status
class KeyValueStoreImpl final : public kvstore::KeyValueStore::Service {
 private:
  // Constructor
  KeyValueStoreImpl() : db_(), terminated_(false), filename_() {}

 public:
  // Destructor
  ~KeyValueStoreImpl() {}

  // Disable move and copy
  KeyValueStoreImpl(const KeyValueStoreImpl&) = delete;
  KeyValueStoreImpl& operator=(const KeyValueStoreImpl&) = delete;

  // Put service
  grpc::Status Put(grpc::ServerContext* context,
                   const kvstore::PutRequest* request,
                   kvstore::PutReply* response) override;

  // Get service
  grpc::Status Get(
      grpc::ServerContext* context,
      grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest>* stream)
      override;

  // Remove service
  grpc::Status Remove(grpc::ServerContext* context,
                      const kvstore::RemoveRequest* request,
                      kvstore::RemoveReply* response) override;

  // Restores data from file and stores data to file upon termination
  bool EnableDiskPersistence(const std::string& filename);

  // The only instance of KeyValueStoreImpl
  static KeyValueStoreImpl& getInstance() {
    static KeyValueStoreImpl instance;
    return instance;
  }

 private:
  // Thread-safe database
  DataBase db_;

  // Flag to stop accepting new requests
  bool terminated_;

  // Name of the file to store
  std::string filename_;

  // Static handler for sigaction. Calls instance's HandlerHelper.
  static void HandleSignal(int s);

  // Loads database from file. Can only be called after persistence is enabled.
  bool Load();

  // Non-static handler for SIGINT and SIGTERM
  bool CleanUp();

  // Saves database to file
  void Save();
};

}  // namespace kvstore

#endif  // KVSTORE_KVSTORE_SERVER_H_
