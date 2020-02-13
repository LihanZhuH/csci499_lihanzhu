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