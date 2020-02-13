#include "warble/warble_func.h"

#include <unordered_map>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "warble/warble.pb.h"
#include "kvstore/kvstore_client.h"

// Test fixture with populated KVStore
// KVStore contains one user "TEST_0"
class WarbleFuncTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage = 
        {{"U:TEST_0", {""}}};
    client_ = std::shared_ptr<kvstore::KVStoreClientAbstract>
                (new kvstore::KVStoreTestClient(storage));
    warble_func_ = std::shared_ptr<warble::WarbleFunc>
                     (new warble::WarbleFunc(client_));
  }

  std::shared_ptr<kvstore::KVStoreClientAbstract> client_;
  std::shared_ptr<warble::WarbleFunc> warble_func_;
};

TEST_F(WarbleFuncTest, SimpleRegister) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_1");
  ASSERT_TRUE(warble_func_->Registeruser(request, &reply));
}

TEST_F(WarbleFuncTest, RegisterExistingUsername) {
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  request.set_username("TEST_0");
  ASSERT_FALSE(warble_func_->Registeruser(request, &reply));
}