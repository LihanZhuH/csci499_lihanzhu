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
    request.set_username("Test_user");
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
    ASSERT_TRUE(status.ok());
    request.set_username("Test_user_1");
    status = warble_func.Registeruser(request, &response);
    ASSERT_FALSE(status.ok());
  }
}

// Test of NewWarble
TEST(WarbleFuncTest, NewWarble) {
  auto channel = grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials());
  {
    // Register a single user
    warble::WarbleFunc warble_func(channel);
    warble::RegisteruserRequest request;
    warble::RegisteruserReply response;
    request.set_username("Test_user");
    auto status = warble_func.Registeruser(request, &response);
    ASSERT_TRUE(status.ok());
  }
  {
    // Post a new warble from existing user
    warble::WarbleFunc warble_func(channel);
    warble::WarbleRequest request;
    warble::WarbleReply response;
    request.set_username("Test_user");
  }
}