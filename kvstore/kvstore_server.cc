#include "kvstore/kvstore_server.h"

#include <glog/logging.h>
#include <signal.h>

#include <iostream>
#include <memory>
#include <thread>

namespace kvstore {

// Defines flag
DEFINE_string(store, "", "Restores/stores data to the given file.");

grpc::Status KeyValueStoreImpl::Put(grpc::ServerContext* context,
                                    const PutRequest* request,
                                    PutReply* response) {
  if (terminated_) {
    return grpc::Status::CANCELLED;
  }
  LOG(INFO) << "kvstore_server - Put: " << request->key() << ", "
            << request->value();
  if (db_.PutIntoStorage(request->key(), request->value())) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status KeyValueStoreImpl::Get(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<GetReply, GetRequest>* stream) {
  if (terminated_) {
    return grpc::Status::CANCELLED;
  }
  GetRequest request;
  stream->Read(&request);
  GetReply reply;
  // Check whether optional is null
  if (auto value_opt = db_.GetFromStorage(request.key())) {
    // Iterate through vector
    for (auto value : *value_opt) {
      reply.set_value(value);
      stream->Write(reply);
      LOG(INFO) << "kvstore_server - Get: " << request.key() << ", " << value;
    }
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status KeyValueStoreImpl::Remove(grpc::ServerContext* context,
                                       const RemoveRequest* request,
                                       RemoveReply* response) {
  if (terminated_) {
    return grpc::Status::CANCELLED;
  }
  if (db_.RemoveFromStorage(request->key())) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

bool KeyValueStoreImpl::EnableDiskPersistence(const std::string& filename) {
  filename_ = filename;

  // Error while reading from file
  if (!Load()) {
    return false;
  }
  // Adds handlers for SIGINT and SIGTERM
  struct sigaction sig_handler;
  sig_handler.sa_flags = 0;
  sig_handler.sa_handler = HandleSignal;
  sigaction(SIGINT, &sig_handler, NULL);
  sigaction(SIGTERM, &sig_handler, NULL);
  return true;
}

void KeyValueStoreImpl::HandleSignal(int s) {
  LOG(INFO) << "kvstore_server - HandleSignal: Storing to file";
  if (!getInstance().CleanUp()) {
    std::cout << "Failed to store to file \"" << getInstance().filename_
              << "\"." << std::endl;
  }
  // Resets signal handlers to SIG_DFL and exits.
  struct sigaction sig_handler;
  sig_handler.sa_flags = 0;
  sig_handler.sa_handler = SIG_DFL;
  sigaction(SIGINT, &sig_handler, NULL);
  sigaction(SIGTERM, &sig_handler, NULL);
  raise(SIGTERM);
}

bool KeyValueStoreImpl::Load() {
  std::ifstream in_file;
  in_file.open(filename_);

  // File doesn't exist
  if (!in_file.is_open()) {
    return true;
  }
  bool result = db_.Deserialize(in_file);
  in_file.close();
  return result;
}

bool KeyValueStoreImpl::CleanUp() {
  terminated_ = true;
  std::ofstream out_file;
  out_file.open(filename_);

  // Fails to open file
  if (out_file.fail()) {
    LOG(WARNING) << "kvstore_server - CleanUp: Failed to open file.";
    return false;
  }
  out_file.close();

  // Starts a new thread for clean up.
  std::thread t(&KeyValueStoreImpl::Save, this);
  t.join();
  return true;
}

void KeyValueStoreImpl::Save() {
  std::ofstream out_file;
  out_file.open(filename_);
  if (!out_file.is_open()) {
    LOG(WARNING) << "kvstore_server - Save: Failed to open file.";
    return;
  }
  db_.Serialize(out_file);
  out_file.close();
}

// Run the server listening on port 50001
int RunServer() {
  std::string server_address("0.0.0.0:50001");
  KeyValueStoreImpl& service = KeyValueStoreImpl::getInstance();

  // Checks if user wants persist storage
  if (!FLAGS_store.empty() && !service.EnableDiskPersistence(FLAGS_store)) {
    std::cout << "Failed to load from file \"" << FLAGS_store << "\"."
              << std::endl
              << "Terminating..." << std::endl;
    LOG(WARNING) << "kvstore_server - RunServer: Failed to restore from file.";
    return -1;
  }

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
  return 0;
}

}  // namespace kvstore

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  return kvstore::RunServer();
}
