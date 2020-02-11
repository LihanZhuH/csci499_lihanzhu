#include "warble_func.h"

namespace warble {

grpc::Status WarbleFunc::Registeruser(const RegisteruserRequest& request,
                                      RegisteruserReply* response) {
  grpc::ClientContext kv_context;
  kvstore::PutRequest kv_request;
  kvstore::PutReply kv_reply;
  std::string username = kUsername + request.username();
  kv_request.set_key(username);
  kv_request.set_value("");

  // Call service function on kvstore server
  grpc::Status status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (status.ok()) {
    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

grpc::Status WarbleFunc::NewWarble(const WarbleRequest& request,
                                   WarbleReply* response) {
  grpc::ClientContext kv_context;
  kvstore::PutRequest kv_request;
  kvstore::PutReply kv_reply;
  std::string username = kUsername + request.username();
  std::string text = kText + request.text();
  std::string parent_id = kReplyTo + request.parent_id();
  std::vector<std::string> value_vec = {username, text, parent_id};
  
  // Also increment warble_cnt_
  kv_request.set_key(kWarbleID + std::to_string(++warble_cnt_));
  
  // Loop through value vector
  for (auto value : value_vec) {
    kv_request.set_value(value);

    // Call service function on kvstore server
    auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
    if (!status.ok()) {
      return grpc::Status::CANCELLED;
    }
  }
  return grpc::Status::OK;
}

grpc::Status WarbleFunc::Follow(const FollowRequest& request,
                                FollowReply* response) {
  grpc::ClientContext kv_context;
  kvstore::PutRequest kv_request;
  kvstore::PutReply kv_reply;

  // Update current user's data
  std::string username = kUsername + request.username();
  std::string to_follow = kFollowing + request.to_follow();
  kv_request.set_key(username);
  kv_request.set_value(to_follow);
  // Call service function on kvstore server
  auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (!status.ok()) {
    return grpc::Status::CANCELLED;
  }

  // Update to_follow's data
  username = kUsername + request.to_follow();
  std::string follower = kFollower + request.username();
  kv_request.set_key(username);
  kv_request.set_value(follower);
  // Call service function on kvstore server
  auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (!status.ok()) {
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status WarbleFunc::Read(const ReadRequest& request,
                              ReadReply* response) {
  grpc::ClientContext kv_context;
  kvstore::GetRequest kv_request;
  kvstore::GetReply kv_reply;

  auto stream = stub_->Get(&kv_context);
  std::string warble_id = kWarbleID + request.warble_id();
  kv_request.set_key(warble_id);
  stream->Write(kv_request);
  // TODO: Read all data of this warble and get replies
  // while (stream->Read(&kv_reply)) {

  // }
  return grpc::Status::CANCELLED;
}

grpc::Status WarbleFunc::Profile(const ProfileRequest& request,
                                 ProfileReply* response) {
  // TODO: Get all followers and followings
  return grpc::Status::CANCELLED;
}

}  // warble