#include "warble_func.h"

#include <glog/logging.h>

namespace warble {

grpc::Status WarbleFunc::Registeruser(const RegisteruserRequest& request,
                                      RegisteruserReply* response) {
  if (UserExists(request.username())) {
    LOG(WARNING) << "WarbleFunc - Registeruser: User already exists";
    return grpc::Status::CANCELLED;  // If user does not exist
  }
  grpc::ClientContext kv_context;
  kvstore::PutRequest kv_request;
  kvstore::PutReply kv_reply;
  std::string username = kUsername + request.username();
  kv_request.set_key(username);
  kv_request.set_value("");

  // Call service function on kvstore server
  auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (status.ok()) {
    return grpc::Status::OK;
  }
  LOG(WARNING) << "WarbleFunc - Registeruser: Invalid Registeruser";
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
  
  // Get current time
  timeval tv;
  gettimeofday(&tv,NULL);
  std::string seconds = kTimestampSec + std::to_string(tv.tv_sec);
  std::string useconds = kTimestampUsec + std::to_string(tv.tv_usec);

  // Vector storing data
  std::vector<std::string> value_vec = {username, text, parent_id,
                                        seconds, useconds};
  
  // Also increment warble_cnt_
  unsigned int new_warble_id_int = ++warble_cnt_;
  kv_request.set_key(kWarbleID + std::to_string(new_warble_id_int));
  
  // Loop through value vector
  for (auto value : value_vec) {
    kv_request.set_value(value);

    // Call service function on kvstore server
    auto status = stub_->Put(&kv_context, kv_request, &kv_reply);
    if (!status.ok()) {
      LOG(WARNING) << "WarbleFunc - NewWarble: Invalid NewWarble";
      return grpc::Status::CANCELLED;
    }
  }
  // Allocate a new Warble and a new timestamp
  Warble* new_warble = new Warble();
  Timestamp* new_time = new Timestamp();

  new_warble->set_id(std::to_string(new_warble_id_int));
  new_warble->set_username(request.username());
  new_warble->set_text(request.text());
  new_warble->set_parent_id(request.parent_id());
  new_time->set_seconds(tv.tv_sec);
  new_time->set_useconds(tv.tv_usec);
  
  new_warble->set_allocated_timestamp(new_time);
  response->set_allocated_warble(new_warble);
  return grpc::Status::OK;
}

grpc::Status WarbleFunc::Follow(const FollowRequest& request,
                                FollowReply* response) {
  if (!UserExists(request.username()) || !UserExists(request.to_follow())) {
    LOG(WARNING) << "WarbleFunc - Follow: User does not exist";
    return grpc::Status::CANCELLED;  // If one of the users does not exist
  }
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
    LOG(WARNING) << "WarbleFunc - Follow: Invalid Follow - 1";
    return grpc::Status::CANCELLED;
  }

  // Update to_follow's data
  username = kUsername + request.to_follow();
  std::string follower = kFollower + request.username();
  kv_request.set_key(username);
  kv_request.set_value(follower);
  // Call service function on kvstore server
  status = stub_->Put(&kv_context, kv_request, &kv_reply);
  if (!status.ok()) {
    LOG(WARNING) << "WarbleFunc - Follow: Invalid Follow - 2";
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status WarbleFunc::Read(const ReadRequest& request,
                              ReadReply* response) {
  ReadHelper(request.warble_id(), response);
  if (response->warbles_size() == 0) {
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status WarbleFunc::Profile(const ProfileRequest& request,
                                 ProfileReply* response) {
  grpc::ClientContext kv_context;
  kvstore::GetRequest kv_request;
  kvstore::GetReply kv_reply;

  // Set up request
  auto stream = stub_->Get(&kv_context);
  std::string username = kUsername + request.username();
  kv_request.set_key(username);
  stream->Write(kv_request);

  // Read all data of this user
  std::string value, qualifier, content;
  while (stream->Read(&kv_reply)) {
    value = kv_reply.value();
    if (value == "" || value.find(':') == std::string::npos) {
      continue;  // This is initialization empty string
    }
    qualifier = value.substr(0, value.find(':')+1);  // Before ':'
    content = value.substr(value.find(':')+1);  // After ':'
    if (qualifier == kFollower) {
      std::string* follower = response->add_followers();
      *follower = content;
    } else if (qualifier == kFollowing) {
      std::string* following = response->add_following();
      *following = content;
    }
  }
  return grpc::Status::CANCELLED;
}

bool WarbleFunc::WarbleExists(const std::string warble_id) {
  grpc::ClientContext kv_context;
  kvstore::GetRequest kv_request;
  kvstore::GetReply kv_reply;

  // Set up request
  auto stream = stub_->Get(&kv_context);
  std::string warble_id_formatted = kWarbleID + warble_id;
  kv_request.set_key(warble_id_formatted);
  stream->Write(kv_request);

  bool warble_exists = false;
  while (stream->Read(&kv_reply)) {
    warble_exists = true;  // Wait for stream to end
  }
  return warble_exists;
}

bool WarbleFunc::UserExists(const std::string username) {
  grpc::ClientContext kv_context;
  kvstore::GetRequest kv_request;
  kvstore::GetReply kv_reply;

  // Set up request
  auto stream = stub_->Get(&kv_context);
  std::string user_formatted = kUsername + username;
  kv_request.set_key(user_formatted);
  stream->Write(kv_request);

  bool user_exists = false;
  while (stream->Read(&kv_reply)) {
    user_exists = true;  // Wait for stream to end
  }
  return user_exists;
}

void WarbleFunc::ReadHelper(const std::string warble_id, ReadReply* response) {
  grpc::ClientContext kv_context;
  kvstore::GetRequest kv_request;
  kvstore::GetReply kv_reply;

  // Set up request
  auto stream = stub_->Get(&kv_context);
  std::string warble_id_formatted = kWarbleID + warble_id;
  kv_request.set_key(warble_id_formatted);
  stream->Write(kv_request);

  Warble* new_warble = response->add_warbles();  // From repeated field
  new_warble->set_id(warble_id);
  Timestamp* main_time = new Timestamp();  // Pre-allocated timestamp
  std::vector<std::string> reply_vec;  // Store all reply warble id

  // Read all data of this warble and get replies
  std::string value, qualifier, content;
  while (stream->Read(&kv_reply)) {
    value = kv_reply.value();
    if (value.find(':') == std::string::npos) {
      LOG(FATAL) << "WarbleFunc - Read: Invalid value field";
    }
    qualifier = value.substr(0, value.find(':')+1);  // Before ':'
    content = value.substr(value.find(':')+1);  // After ':'
    if (qualifier == kUsername) {
      new_warble->set_username(content);
    } else if (qualifier == kText) {
      new_warble->set_text(content);
    } else if (qualifier == kReplyTo) {
      new_warble->set_parent_id(content);
    } else if (qualifier == kTimestampSec) {
      main_time->set_seconds(std::stoi(content));
    } else if (qualifier == kTimestampUsec) {
      main_time->set_useconds(std::stoi(content));
    } else {
      reply_vec.push_back(content);
    }
  }
  new_warble->set_allocated_timestamp(main_time);

  // Get warble from reply warbles
  for (auto reply_id : reply_vec) {
    ReadHelper(reply_id, response);
  }
}

}  // warble