#include "warble/warble_func.h"

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "kvstore/kvstore_client.h"
#include "warble/warble.pb.h"

// Test fixture with basic populated KVStore
// KVStore contains two users "TEST_0" and "TEST_00"
class WarbleFuncBasicTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage = {
        {"U:TEST_0", {""}}, {"U:TEST_00", {""}}};
    client_ = std::shared_ptr<kvstore::KVStoreClientAbstract>(
        new kvstore::KVStoreTestClient(storage));
  }

  std::shared_ptr<kvstore::KVStoreClientAbstract> client_;
};

// Registering a new user
// Should return true
TEST_F(WarbleFuncBasicTest, SimpleRegisterShouldSucceed) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_1");
  ASSERT_TRUE(Registeruser(client_, request, &reply));
}

// Registering an existing user
// Should return false
TEST_F(WarbleFuncBasicTest, RegisterExistingUsernameShouldFail) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_0");
  ASSERT_FALSE(Registeruser(client_, request, &reply));
}

// Registering a user twice
// The first attempt should return true, and the second one should return false
TEST_F(WarbleFuncBasicTest, RegisterAUserTwiceShouldFail) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_1");
  ASSERT_TRUE(Registeruser(client_, request, &reply));
  ASSERT_FALSE(Registeruser(client_, request, &reply));
}

// Creating a new warble from existing user
// Should return true
TEST_F(WarbleFuncBasicTest, SimpleNewWarbleShouldSucceed) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(NewWarble(client_, request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text");
}

// Creating a new warble from unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, NewWarbleFromUnknownUserShouldFail) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_1");
  request.set_text("text");
  ASSERT_FALSE(NewWarble(client_, request, &reply));
}

// Creating two new warbles from existing user
// Should return true
TEST_F(WarbleFuncBasicTest, DoubleNewWarbleShouldSucceed) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;

  // First warble
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(NewWarble(client_, request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text");

  // Second warble
  request.set_username("TEST_0");
  request.set_text("text_1");
  ASSERT_TRUE(NewWarble(client_, request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text_1");
}

// Following an existing user
// Should return true and update info
TEST_F(WarbleFuncBasicTest, FollowExistingUserShouldSucceed) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_0");
  request.set_to_follow("TEST_00");
  ASSERT_TRUE(Follow(client_, request, &reply));
}

// Following an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, FollowUnknownUserShouldFail) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_0");
  request.set_to_follow("TEST_1");
  ASSERT_FALSE(Follow(client_, request, &reply));
}

// Following from an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, UnknownFollowUserShouldFail) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_1");
  request.set_to_follow("TEST_0");
  ASSERT_FALSE(Follow(client_, request, &reply));
}

// Reading after posting a new warble
// Should get the same warble
TEST_F(WarbleFuncBasicTest, BasicReadShouldSucceed) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(NewWarble(client_, request, &reply));

  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id(reply.warble().id());
  ASSERT_TRUE(Read(client_, read_request, &read_reply));
  ASSERT_EQ(read_reply.warbles().size(), 1);
  warble::Warble warble_obj = read_reply.warbles().at(0);
  ASSERT_EQ(warble_obj.username(), "TEST_0");
  ASSERT_EQ(warble_obj.text(), "text");
}

// Reading unknown warble
// Should return false
TEST_F(WarbleFuncBasicTest, ReadUnknownShouldFail) {
  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id("1");
  ASSERT_FALSE(Read(client_, read_request, &read_reply));
}

// Getting the profile of an existing user
// Should get empty follower and following
TEST_F(WarbleFuncBasicTest, BasicProfileShouldSucceed) {
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  request.set_username("TEST_0");
  ASSERT_TRUE(Profile(client_, request, &reply));
  ASSERT_EQ(reply.followers().size(), 0);
  ASSERT_EQ(reply.following().size(), 0);
}

// Getting the profile of an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, UnknownProfileShouldFail) {
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  request.set_username("TEST_1");
  ASSERT_FALSE(Profile(client_, request, &reply));
}

// Test fixture with complex populated KVStore
// KVStore contains two users "TEST_0" and "TEST_00"
// TEST_0 follows TEST_00
// TEST_0 has posted a warble id 1
class WarbleFuncComplexTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage = {
        {"U:TEST_0", {"FI:TEST_00"}}, {"U:TEST_00", {"FE:TEST_0"}}};
    client_ = std::shared_ptr<kvstore::KVStoreClientAbstract>(
        new kvstore::KVStoreTestClient(storage));

    // Create some warbles
    warble::WarbleRequest request;
    warble::WarbleReply reply;
    request.set_username("TEST_0");
    request.set_text("text_0");
    NewWarble(client_, request, &reply);
    parent_id_ = reply.warble().id();
  }

  std::shared_ptr<kvstore::KVStoreClientAbstract> client_;
  std::string parent_id_;
};

// Replying to existing warble
// Should return true
TEST_F(WarbleFuncComplexTest, NewWarbleReplyShouldSucceed) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text_1");
  request.set_parent_id(parent_id_);
  ASSERT_TRUE(NewWarble(client_, request, &reply));
}

// Replying to existing warble, and read the thread from parent
// Should return true
TEST_F(WarbleFuncComplexTest, ReadThreadShouldSucceed) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_00");
  request.set_text("text_1");
  request.set_parent_id(parent_id_);
  ASSERT_TRUE(NewWarble(client_, request, &reply));

  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id(parent_id_);
  ASSERT_TRUE(Read(client_, read_request, &read_reply));
  ASSERT_EQ(read_reply.warbles().size(), 2);  // Should contain two warbles
  // Compare content
  EXPECT_EQ(read_reply.warbles().at(0).username(), "TEST_0");
  EXPECT_EQ(read_reply.warbles().at(0).text(), "text_0");
  EXPECT_EQ(read_reply.warbles().at(1).username(), "TEST_00");
  EXPECT_EQ(read_reply.warbles().at(1).text(), "text_1");
  EXPECT_EQ(read_reply.warbles().at(1).parent_id(), parent_id_);
}
