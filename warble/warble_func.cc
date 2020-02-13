#include "warble_func.h"

#include <glog/logging.h>

namespace warble {

bool WarbleFunc::Registeruser(const RegisteruserRequest& request,
                                      RegisteruserReply* response) {
  if (kvstore_client_->KeyExists(kUsername+request.username())) {
    LOG(WARNING) << "WarbleFunc - Registeruser: User already exists";
    return false;  // If user exists
  }
  std::string username = kUsername + request.username();
  if (kvstore_client_->Put(username, "")) {
    return true;
  }
  LOG(WARNING) << "WarbleFunc - Registeruser: Invalid Registeruser";
  return false;
}

bool WarbleFunc::NewWarble(const WarbleRequest& request,
                                   WarbleReply* response) {
  if (!kvstore_client_->KeyExists(kUsername+request.username())) {
    LOG(WARNING) << "WarbleFunc - NewWarble: User does not exist";
    return false;  // If user does not exist
  }
  std::string username = kUsername + request.username();
  std::string text = kText + request.text();
  std::string parent_id = kReplyTo + request.parent_id();
  
  // Get current time
  timeval tv;
  gettimeofday(&tv,NULL);
  std::string seconds = kTimestampSec + std::to_string(tv.tv_sec);
  std::string useconds = kTimestampUsec + std::to_string(tv.tv_usec);

  // Vector storing data
  std::vector<std::string> value_vec = {username, text,
                                        seconds, useconds};
  unsigned int new_warble_id_int = warble_cnt_ + 1;

  // Check if request is a reply
  if (!request.parent_id().empty()) {
    value_vec.push_back(parent_id);

    // Check if the parent id is a valid warble
    if (!kvstore_client_->KeyExists(kWarbleID + request.parent_id())) {
      LOG(WARNING) << "WarbleFunc - NewWarble: Invalid parent id";
      return false;
    }

    // Add reply to parent warble
    std::string key = kWarbleID + request.parent_id();
    std::string value = kReplies + std::to_string(new_warble_id_int);
    if (!kvstore_client_->Put(key, value)) {
      LOG(WARNING) << "WarbleFunc - NewWarble: Invalid reply";
      return false;
    }
  }
  std::string key;
  // Loop through value vector
  for (auto value : value_vec) {
    key = kWarbleID + std::to_string(new_warble_id_int);
    if (!kvstore_client_->Put(key, value)) {
      LOG(WARNING) << "WarbleFunc - NewWarble: Invalid NewWarble";
      return false;
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

  ++warble_cnt_;  // Valid warble
  return true;
}

bool WarbleFunc::Follow(const FollowRequest& request,
                                FollowReply* response) {
  if (!kvstore_client_->KeyExists(kUsername+request.username()) ||
      !kvstore_client_->KeyExists(kUsername+request.to_follow())) {
    LOG(WARNING) << "WarbleFunc - Follow: User does not exist";
    return false;  // If one of the users does not exist
  }
  // Update current user's data
  std::string username = kUsername + request.username();
  std::string to_follow = kFollowing + request.to_follow();
  if (!kvstore_client_->Put(username, to_follow)) {
    LOG(WARNING) << "WarbleFunc - Follow: Invalid Follow - 1";
    return false;
  }

  // Update to_follow's data
  username = kUsername + request.to_follow();
  std::string follower = kFollower + request.username();
  if (!kvstore_client_->Put(username, follower)) {
    LOG(WARNING) << "WarbleFunc - Follow: Invalid Follow - 2";
    return false;
  }
  return true;
}

bool WarbleFunc::Read(const ReadRequest& request,
                              ReadReply* response) {
  std::string warble_id = request.warble_id();
  std::queue<std::string> warble_id_queue;  // Queue of warbles in a thread
  warble_id_queue.push(warble_id);

  // Loop through all replies in a thread
  while (!warble_id_queue.empty()) {
    std::string warble_id_formatted = kWarbleID + warble_id_queue.front();
    if (!kvstore_client_->KeyExists(warble_id_formatted)) {
      LOG(WARNING) << "WarbleFunc - Read: Invalid warble ID";
      return false;
    }
    warble_id_queue.pop();
    auto value_vec = kvstore_client_->Get(warble_id_formatted);

    Warble* new_warble = response->add_warbles();  // From repeated field
    new_warble->set_id(warble_id);
    Timestamp* new_time = new Timestamp();  // Pre-allocated timestamp
    
    // Read all data of this warble and get replies
    std::string value, qualifier, content;
    for (auto value : value_vec) {
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
        new_time->set_seconds(std::stoi(content));
      } else if (qualifier == kTimestampUsec) {
        new_time->set_useconds(std::stoi(content));
      } else {
        warble_id_queue.push(content);
      }
    }
    new_warble->set_allocated_timestamp(new_time);
  }

  if (response->warbles_size() == 0) {
    return false;
  }
  return true;
}

bool WarbleFunc::Profile(const ProfileRequest& request,
                                 ProfileReply* response) {
  std::string username = kUsername + request.username();
  if (!kvstore_client_->KeyExists(username)) {
    LOG(WARNING) << "WarbleFunc - Profile: User does not exist";
    return false;  // If user does not exist
  }
  // Set up request
  auto value_vec = kvstore_client_->Get(username);

  // Read all data of this user
  std::string value, qualifier, content;
  for (auto value : value_vec) {
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
  return true;
}

}  // warble