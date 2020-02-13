#include "kvstore/kvstore_client.h"

#include <glog/logging.h>

namespace kvstore {

// Implementation of KVStoreWarbleClient

bool KVStoreWarbleClient::Put(const std::string& key,
                              const std::string& value) {
  grpc::ClientContext kv_context;
  PutRequest kv_request;
  PutReply kv_reply;
  kv_request.set_key(key);
  kv_request.set_value(value);

  // Call service function on kvstore server
  auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (status.ok()) {
    return true;
  }
  return false;
}

std::vector<std::string> KVStoreWarbleClient::Get(const std::string& key) {
  grpc::ClientContext kv_context;
  GetRequest kv_request;
  GetReply kv_reply;
  std::vector<std::string> result;

   // Set up request
  auto stream = stub_->Get(&kv_context);
  kv_request.set_key(key);
  stream->Write(kv_request);

  // Read all data of this user
  while (stream->Read(&kv_reply)) {
    result.push_back(kv_reply.value());
  }
  return result;
}

bool KVStoreWarbleClient::Remove(const std::string& key) {
  grpc::ClientContext kv_context;
  RemoveRequest kv_request;
  RemoveReply kv_reply;
  kv_request.set_key(key);

  // Call service function on kvstore server
  auto status = stub_->Remove(&kv_context, kv_request, &kv_reply);
  if (status.ok()) {
    return true;
  }
  return false;
}

bool KVStoreWarbleClient::KeyExists(const std::string& key) {
  grpc::ClientContext kv_context;
  GetRequest kv_request;
  GetReply kv_reply;

  // Set up request
  auto stream = stub_->Get(&kv_context);
  kv_request.set_key(key);
  stream->Write(kv_request);

  bool key_exists = false;
  while (stream->Read(&kv_reply)) {
    key_exists = true;  // Wait for stream to end
  }
  return key_exists;
}

// Implementation of KVStoreTestClient

bool KVStoreTestClient::Put(const std::string& key,
                            const std::string& value) {
  auto it = std::find(storage_[key].begin(),
                      storage_[key].end(),
                      value);
  if (it != storage_[key].end()) {
    return false;
  }
  storage_[key].push_back(value);
  return true;
}

std::vector<std::string> KVStoreTestClient::Get(const std::string& key) {
  if (storage_.count(key) > 0 && !storage_[key].empty()) {
    return storage_[key];
  }
  return {};
}

bool KVStoreTestClient::Remove(const std::string& key) {
  if (storage_.count(key) > 0) {
    storage_.erase(key);
    return true;
  }
  return false;
}

bool KVStoreTestClient::KeyExists(const std::string& key) {
  auto it = storage_.find(key);
  if (it == storage_.end()) {
    return false;
  }
  return true;
}

}  // kvstore