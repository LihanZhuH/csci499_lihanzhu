#ifndef WARBLE_WARBLE_FUNC_H_
#define WARBLE_WARBLE_FUNC_H_

#include <sys/time.h>

#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "kvstore/kvstore_client.h"
#include "warble/warble.pb.h"

namespace warble {

typedef std::shared_ptr<kvstore::KVStoreClientAbstract> KVStoreClientPtr;

// All warble functions take in a pointer to KVStoreClientAbstract,
// and use Protobuf for all requests and responses.
// Success/failure is signaled via returned boolean.

// Register the given non-blank username
bool Registeruser(KVStoreClientPtr kvstore_client,
                  const RegisteruserRequest& request,
                  RegisteruserReply* response);

// Post a new warble (optionally as a reply), return the id of the new warble
bool NewWarble(KVStoreClientPtr kvstore_client, const WarbleRequest& request,
               WarbleReply* response);

// Start following a given user
bool Follow(KVStoreClientPtr kvstore_client, const FollowRequest& request,
            FollowReply* response);

// Read a warble thread from the given id
bool Read(KVStoreClientPtr kvstore_client, const ReadRequest& request,
          ReadReply* response);

// Return this user’s following and followers
bool Profile(KVStoreClientPtr kvstore_client, const ProfileRequest& request,
             ProfileReply* response);

// Random generator for warble id
static std::mt19937 rand(std::random_device{}());

// Prefix for database strings
static std::string kUsername = "U:";
static std::string kFollowing = "FI:";
static std::string kFollower = "FE:";
static std::string kWarbleID = "W:";
static std::string kText = "T:";
static std::string kReplyTo = "RT:";
static std::string kReplies = "RS:";
static std::string kTimestampSec = "TS:";
static std::string kTimestampUsec = "TU:";

}  // namespace warble

#endif  // WARBLE_WARBLE_FUNC_H_
