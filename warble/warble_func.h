#ifndef WARBLE_FUNC_H_
#define WARBLE_FUNC_H_

#include <string>
#include <memory>
#include <vector>
#include <sys/time.h>

#include <grpcpp/grpcpp.h>

#include "warble/warble.pb.h"
#include "kvstore/kvstore.grpc.pb.h"
#include "kvstore/kvstore.pb.h"

namespace warble {

// Warble function calls on Func platform
// Use Protobuf for all requests and responses
// Success/failure is signaled via returned grpc::Status
class WarbleFunc {
 public:
  // Constructor that requires a grpc Channel for kvstore server
  WarbleFunc(std::shared_ptr<grpc::Channel> channel)
      : stub_(kvstore::KeyValueStore::NewStub(channel)), warble_cnt_(0) {}

  // Disable move and copy
  WarbleFunc(const WarbleFunc&) = delete;
  WarbleFunc& operator=(const WarbleFunc&) = delete;

  // Register the given non-blank username
  grpc::Status Registeruser(const RegisteruserRequest& request,
                            RegisteruserReply* response);

  // Post a new warble (optionally as a reply), return the id of the new warble
  grpc::Status NewWarble(const WarbleRequest& request, WarbleReply* response);

  // Start following a given user
  grpc::Status Follow(const FollowRequest& request, FollowReply* response);

  // Read a warble thread from the given id
  grpc::Status Read(const ReadRequest& request, ReadReply* response);

  // Return this user’s following and followers
  grpc::Status Profile(const ProfileRequest& request, ProfileReply* response);

 private:
  // Return true if the warble with given id exists
  bool WarbleExists(const std::string& warble_id);

  // Return true if the user with given username exists
  bool UserExists(const std::string& username);
  
  // Recursively read a thread and write to response
  void ReadHelper(const std::string& warble_id, ReadReply* response);
  
  // Pointer of stub for connection to kvstore server
  std::unique_ptr<kvstore::KeyValueStore::Stub> stub_;

  // Count total warbles and generate new warble id
  unsigned int warble_cnt_;

  // Prefix for database strings
  const std::string kUsername = "U:";
  const std::string kFollowing = "FI:";
  const std::string kFollower = "FE:";
  const std::string kWarbleID = "W:";
  const std::string kText = "T:";
  const std::string kReplyTo = "RT:";
  const std::string kReplies = "RS:";
  const std::string kTimestampSec = "TS:";
  const std::string kTimestampUsec = "TU:";
};

}  // warble

#endif  // WARBLE_FUNC_H_