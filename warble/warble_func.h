#ifndef WARBLE_FUNC_H_
#define WARBLE_FUNC_H_

#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <sys/time.h>

#include "warble/warble.pb.h"
#include "kvstore/kvstore_client.h"

namespace warble {

// Warble function calls on Func platform
// Use Protobuf for all requests and responses
// Success/failure is signaled via returned boolean
class WarbleFunc {
 public:
  // Constructor that requires a pointer to KVStore client
  WarbleFunc(std::shared_ptr<kvstore::KVStoreClientAbstract> client)
      : kvstore_client_(client), warble_cnt_(0) {}

  // Disable move and copy
  WarbleFunc(const WarbleFunc&) = delete;
  WarbleFunc& operator=(const WarbleFunc&) = delete;

  // Register the given non-blank username
  bool Registeruser(const RegisteruserRequest& request,
                            RegisteruserReply* response);

  // Post a new warble (optionally as a reply), return the id of the new warble
  bool NewWarble(const WarbleRequest& request, WarbleReply* response);

  // Start following a given user
  bool Follow(const FollowRequest& request, FollowReply* response);

  // Read a warble thread from the given id
  bool Read(const ReadRequest& request, ReadReply* response);

  // Return this user’s following and followers
  bool Profile(const ProfileRequest& request, ProfileReply* response);

 private:
  // Pointer to KVStore client that communicates to database
  std::shared_ptr<kvstore::KVStoreClientAbstract> kvstore_client_;

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