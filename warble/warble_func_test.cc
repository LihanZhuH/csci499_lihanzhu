#include "warble/warble_func.h"

#include <gtest/gtest.h>
#include <grpcpp/grpcpp.h>

#include "warble/warble.pb.h"

// Before testing, please make sure kvstore_server is running on port 50001

// Test of registeruser
TEST(WarbleFuncTest, Registeruser) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  {
    // Register a single user
    warble::WarbleFunc warble_func(channel);
    warble::RegisteruserRequest request;
    warble::RegisteruserReply response;
    request.set_username("Test_user_0");
    auto status = warble_func.Registeruser(request, &response);
    ASSERT_TRUE(status.ok());
  }
  {
    // Register two differnet users
    warble::WarbleFunc warble_func(channel);
    warble::RegisteruserRequest request;
    warble::RegisteruserReply response;
    request.set_username("Test_user_1");
    auto status = warble_func.Registeruser(request, &response);
    ASSERT_TRUE(status.ok());
    request.set_username("Test_user_2");
    status = warble_func.Registeruser(request, &response);
    ASSERT_TRUE(status.ok());
  }
  {
    // Register two same users
    warble::WarbleFunc warble_func(channel);
    warble::RegisteruserRequest request;
    warble::RegisteruserReply response;
    request.set_username("Test_user_1");
    auto status = warble_func.Registeruser(request, &response);
    EXPECT_FALSE(status.ok());
  }
}

// Test of NewWarble
TEST(WarbleFuncTest, NewWarble) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  warble::WarbleFunc warble_func(channel);
  {
    // Post a new warble from existing user
    warble::WarbleRequest warble_request;
    warble::WarbleReply warble_response;
    warble_request.set_username("Test_user_0");
    warble_request.set_text("Text");
    auto status = warble_func.NewWarble(warble_request, &warble_response);
    ASSERT_TRUE(status.ok());

    // Compare reply and input
    warble::Warble reply_warble = warble_response.warble();
    EXPECT_EQ(reply_warble.username(), "Test_user_0");
    EXPECT_EQ(reply_warble.text(), "Text");
    EXPECT_EQ(reply_warble.id(), "1");
  }
  {
    // Post a new warble from non-existing user
    warble::WarbleRequest warble_request;
    warble::WarbleReply warble_response;
    warble_request.set_username("Test_user_?");
    warble_request.set_text("Text");
    auto status = warble_func.NewWarble(warble_request, &warble_response);
    ASSERT_FALSE(status.ok());
  }
  {
    // Post a new warble in reply to warble id 1
    warble::WarbleRequest warble_request;
    warble::WarbleReply warble_response;
    warble_request.set_username("Test_user_1");
    warble_request.set_text("Reply");
    warble_request.set_parent_id("1");
    auto status = warble_func.NewWarble(warble_request, &warble_response);
    ASSERT_TRUE(status.ok());

    // Compare current user
    warble::Warble reply_warble = warble_response.warble();
    EXPECT_EQ(reply_warble.username(), "Test_user_1");
    EXPECT_EQ(reply_warble.text(), "Reply");
    EXPECT_EQ(reply_warble.id(), "2");
    EXPECT_EQ(reply_warble.parent_id(), "1");
  }
}

// Test of Follow
TEST(WarbleFuncTest, Follow) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  warble::WarbleFunc warble_func(channel);
  {
    // Basic follow
    warble::FollowRequest follow_request;
    warble::FollowReply follow_reply;
    follow_request.set_username("Test_user_0");
    follow_request.set_to_follow("Test_user_1");
    auto status = warble_func.Follow(follow_request, &follow_reply);
    ASSERT_TRUE(status.ok());
  }
  {
    // Follow non-existing
    warble::FollowRequest follow_request;
    warble::FollowReply follow_reply;
    follow_request.set_username("Test_user_0");
    follow_request.set_to_follow("Test_user_?");
    auto status = warble_func.Follow(follow_request, &follow_reply);
    ASSERT_FALSE(status.ok());
  }
  {
    // Follow from non-existing current user
    warble::FollowRequest follow_request;
    warble::FollowReply follow_reply;
    follow_request.set_username("Test_user_?");
    follow_request.set_to_follow("Test_user_1");
    auto status = warble_func.Follow(follow_request, &follow_reply);
    ASSERT_FALSE(status.ok());
  }
}

// Test of Read
TEST(WarbleFuncTest, Read) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  warble::WarbleFunc warble_func(channel);
  {
    // Read existing
    warble::ReadRequest read_request;
    warble::ReadReply read_reply;
    read_request.set_warble_id("1");
    auto status = warble_func.Read(read_request, &read_reply);
    ASSERT_TRUE(status.ok());

    // Check content
    ASSERT_EQ(read_reply.warbles_size(), 2);
    warble::Warble reply_warble = read_reply.warbles(0);
    EXPECT_EQ(reply_warble.username(), "Test_user_0");
    EXPECT_EQ(reply_warble.text(), "Text");
    EXPECT_EQ(reply_warble.id(), "1");

    reply_warble = read_reply.warbles(1);
    EXPECT_EQ(reply_warble.username(), "Test_user_1");
    EXPECT_EQ(reply_warble.text(), "Reply");
    EXPECT_EQ(reply_warble.id(), "2");
    EXPECT_EQ(reply_warble.parent_id(), "1");
  }
}

// Test of Profile
TEST(WarbleFuncTest, Profile) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  warble::WarbleFunc warble_func(channel);
  {
    // Basic profile (using existing database)
    warble::ProfileRequest profile_request;
    warble::ProfileReply profile_reply;
    profile_request.set_username("Test_user_0");
    auto status = warble_func.Profile(profile_request, &profile_reply);
    ASSERT_TRUE(status.ok());

    // Compare result
    ASSERT_EQ(profile_reply.followers_size(), 0);
    ASSERT_EQ(profile_reply.following_size(), 1);
    EXPECT_EQ(profile_reply.following(0), "Test_user_1");
  }
  {
    // Profile of non-existing user
    warble::ProfileRequest profile_request;
    warble::ProfileReply profile_reply;
    profile_request.set_username("Test_user_?");
    auto status = warble_func.Profile(profile_request, &profile_reply);
    ASSERT_FALSE(status.ok());
  }
  {
    // Complex follower and following
    // Add more following and followers
    warble::FollowRequest follow_request;
    warble::FollowReply follow_reply;
    follow_request.set_username("Test_user_0");
    follow_request.set_to_follow("Test_user_2");
    auto status = warble_func.Follow(follow_request, &follow_reply);
    ASSERT_TRUE(status.ok());  // Now user_0 follows user_1 and _2

    follow_request.set_username("Test_user_1");
    follow_request.set_to_follow("Test_user_0");
    status = warble_func.Follow(follow_request, &follow_reply);
    ASSERT_TRUE(status.ok());  // Now user_0 has 1 follower user_1

    // Get profile of user_0
    warble::ProfileRequest profile_request;
    warble::ProfileReply profile_reply;
    profile_request.set_username("Test_user_0");
    status = warble_func.Profile(profile_request, &profile_reply);
    ASSERT_TRUE(status.ok());

    // Compare result
    ASSERT_EQ(profile_reply.followers_size(), 1);
    ASSERT_EQ(profile_reply.following_size(), 2);
    EXPECT_EQ(profile_reply.following(0), "Test_user_1");
    EXPECT_EQ(profile_reply.following(1), "Test_user_2");
    EXPECT_EQ(profile_reply.followers(0), "Test_user_1");
  }
}