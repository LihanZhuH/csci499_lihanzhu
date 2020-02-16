#ifndef KVSTORE_CLIENT_
#define KVSTORE_CLIENT_

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <unordered_map>

#include <grpcpp/grpcpp.h>

#include "kvstore/kvstore.grpc.pb.h"
#include "kvstore/kvstore.pb.h"

namespace kvstore {

// Base class for KVStore client / test fixture
class KVStoreClientAbstract {
 public:
  KVStoreClientAbstract() {}
  virtual ~KVStoreClientAbstract() {}

  // Put key and value into KVStore. Success/failure signaled via returned bool
  virtual bool Put(const std::string& key, const std::string& value) = 0;

  // Get an optional vector of values given a key. Return null if not found
  virtual std::vector<std::string> Get(const std::string& key) = 0;

  // Remove all values of a key
  virtual bool Remove(const std::string& key) = 0;

  // Checks whether a key exists
  virtual bool KeyExists(const std::string& key) = 0;
};

// KVStore client that communicates with GRPC server
class KVStoreWarbleClient : public KVStoreClientAbstract {
 public:
  KVStoreWarbleClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}

  // Disable move and copy
  KVStoreWarbleClient(const KVStoreWarbleClient&) = delete;
  KVStoreWarbleClient& operator=(const KVStoreWarbleClient&) = delete;
  
  // Put key and value into KVStore. Success/failure signaled via returned bool
  bool Put(const std::string& key, const std::string& value);

  // Get an optional vector of values given a key. Return null if not found
  std::vector<std::string> Get(const std::string& key);

  // Remove all values of a key
  bool Remove(const std::string& key);

  // Checks whether a key exists
  bool KeyExists(const std::string& key);

 private:
  // Pointer of stub for connection to kvstore server
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

// KVStore client that uses local data for testing
class KVStoreTestClient : public KVStoreClientAbstract {
 public:
  KVStoreTestClient() {}
  KVStoreTestClient(const std::unordered_map<std::string, std::vector<std::string>>& storage)
      : storage_(storage){}

  // Disable move and copy
  KVStoreTestClient(const KVStoreTestClient&) = delete;
  KVStoreTestClient& operator=(const KVStoreTestClient&) = delete;

  // Put key and value into KVStore. Success/failure signaled via returned bool
  bool Put(const std::string& key, const std::string& value);

  // Get an optional vector of values given a key. Return null if not found
  std::vector<std::string> Get(const std::string& key);

  // Remove all values of a key
  bool Remove(const std::string& key);

  // Checks whether a key exists
  bool KeyExists(const std::string& key);

 private:
  // Local storage using a map
  std::unordered_map<std::string, std::vector<std::string>> storage_;
};

}  // kvstore

#endif  // KVSTORE_CLIENT_