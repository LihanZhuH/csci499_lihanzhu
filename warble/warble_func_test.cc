#include "warble/warble_func.h"

#include <unordered_map>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "warble/warble.pb.h"
#include "kvstore/kvstore_client.h"

// Test fixture with basic populated KVStore
// KVStore contains two users "TEST_0" and "TEST_00"
class WarbleFuncBasicTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage = 
        {{"U:TEST_0", {""}},
         {"U:TEST_00", {""}}};
    client_ = std::shared_ptr<kvstore::KVStoreClientAbstract>
                (new kvstore::KVStoreTestClient(storage));
    warble_func_ = std::shared_ptr<warble::WarbleFunc>
                     (new warble::WarbleFunc(client_));
  }

  std::shared_ptr<kvstore::KVStoreClientAbstract> client_;
  std::shared_ptr<warble::WarbleFunc> warble_func_;
};

// Registering a new user
// Should return false
TEST_F(WarbleFuncBasicTest, SimpleRegister) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_1");
  ASSERT_TRUE(warble_func_->Registeruser(request, &reply));
}

// Registering an existing user
// Should return false
TEST_F(WarbleFuncBasicTest, RegisterExistingUsername) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_0");
  ASSERT_FALSE(warble_func_->Registeruser(request, &reply));
}

// Registering a user twice
// The first attempt should return true, and the second one should return false
TEST_F(WarbleFuncBasicTest, RegisterAUserTwice) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_1");
  ASSERT_TRUE(warble_func_->Registeruser(request, &reply));
  ASSERT_FALSE(warble_func_->Registeruser(request, &reply));
}

// Creating a new warble from existing user
// Should return true
TEST_F(WarbleFuncBasicTest, SimpleNewWarble) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().id(), "1");
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text");
}

// Creating a new warble from unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, NewWarbleFromUnknownUser) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_1");
  request.set_text("text");
  ASSERT_FALSE(warble_func_->NewWarble(request, &reply));
}

// Creating two new warbles from existing user
// Should return true
TEST_F(WarbleFuncBasicTest, DoubleNewWarble) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;

  // First warble
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().id(), "1");
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text");

  // Second warble
  request.set_username("TEST_0");
  request.set_text("text_1");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().id(), "2");
  EXPECT_EQ(reply.warble().username(), "TEST_0");
  EXPECT_EQ(reply.warble().text(), "text_1");
}

// Following an existing user
// Should return true and update info
TEST_F(WarbleFuncBasicTest, FollowExistingUser) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_0");
  request.set_to_follow("TEST_00");
  ASSERT_TRUE(warble_func_->Follow(request, &reply));
}

// Following an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, FollowUnknownUser) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_0");
  request.set_to_follow("TEST_1");
  ASSERT_FALSE(warble_func_->Follow(request, &reply));
}

// Following from an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, UnknownFollowUser) {
  warble::FollowRequest request;
  warble::FollowReply reply;
  request.set_username("TEST_1");
  request.set_to_follow("TEST_0");
  ASSERT_FALSE(warble_func_->Follow(request, &reply));
}

// Reading after posting a new warble
// Should get the same warble
TEST_F(WarbleFuncBasicTest, BasicRead) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));

  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id("1");
  ASSERT_TRUE(warble_func_->Read(read_request, &read_reply));
  ASSERT_EQ(read_reply.warbles().size(), 1);
  warble::Warble warble_obj = read_reply.warbles().at(0);
  ASSERT_EQ(warble_obj.username(), "TEST_0");
  ASSERT_EQ(warble_obj.text(), "text");
  ASSERT_EQ(warble_obj.id(), "1");
}

// Reading unknown warble
// Should return false
TEST_F(WarbleFuncBasicTest, ReadUnknown) {
  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id("1");
  ASSERT_FALSE(warble_func_->Read(read_request, &read_reply));
}

// Getting the profile of an existing user
// Should get empty follower and following
TEST_F(WarbleFuncBasicTest, BasicProfile) {
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  request.set_username("TEST_0");
  ASSERT_TRUE(warble_func_->Profile(request, &reply));
  ASSERT_EQ(reply.followers().size(), 0);
  ASSERT_EQ(reply.following().size(), 0);
}

// Getting the profile of an unknown user
// Should return false
TEST_F(WarbleFuncBasicTest, UnknownProfile) {
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  request.set_username("TEST_1");
  ASSERT_FALSE(warble_func_->Profile(request, &reply));
}

// Test fixture with complex populated KVStore
// KVStore contains two users "TEST_0" and "TEST_00"
// TEST_0 follows TEST_00
// TEST_0 has posted a warble id 1
class WarbleFuncComplexTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage = 
        {{"U:TEST_0", {"FI:TEST_00"}},
         {"U:TEST_00", {"FE:TEST_0"}}};
    client_ = std::shared_ptr<kvstore::KVStoreClientAbstract>
                (new kvstore::KVStoreTestClient(storage));
    warble_func_ = std::shared_ptr<warble::WarbleFunc>
                     (new warble::WarbleFunc(client_));

    // Create some warbles
    warble::WarbleRequest request;
    warble::WarbleReply reply;
    request.set_username("TEST_0");
    request.set_text("text_0");
    warble_func_->NewWarble(request, &reply);
  }

  std::shared_ptr<kvstore::KVStoreClientAbstract> client_;
  std::shared_ptr<warble::WarbleFunc> warble_func_;
};

// Replying to existing warble
// Should return true
TEST_F(WarbleFuncComplexTest, NewWarbleReply) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_0");
  request.set_text("text_1");
  request.set_parent_id("1");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().id(), "2");
}

// Replying to existing warble, and read the thread from parent
// Should return true
TEST_F(WarbleFuncComplexTest, ReadThread) {
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  request.set_username("TEST_00");
  request.set_text("text_1");
  request.set_parent_id("1");
  ASSERT_TRUE(warble_func_->NewWarble(request, &reply));
  // Compare warble content
  EXPECT_EQ(reply.warble().id(), "2");

  warble::ReadRequest read_request;
  warble::ReadReply read_reply;
  read_request.set_warble_id("1");
  ASSERT_TRUE(warble_func_->Read(read_request, &read_reply));
  ASSERT_EQ(read_reply.warbles().size(), 2);  // Should contain two warbles
  // Compare content
  EXPECT_EQ(read_reply.warbles().at(0).id(), "1");
  EXPECT_EQ(read_reply.warbles().at(0).username(), "TEST_0");
  EXPECT_EQ(read_reply.warbles().at(0).text(), "text_0");
  EXPECT_EQ(read_reply.warbles().at(1).id(), "2");
  EXPECT_EQ(read_reply.warbles().at(1).username(), "TEST_00");
  EXPECT_EQ(read_reply.warbles().at(1).text(), "text_1");
  EXPECT_EQ(read_reply.warbles().at(1).parent_id(), "1");
}