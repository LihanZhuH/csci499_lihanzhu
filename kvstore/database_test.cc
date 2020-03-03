#include "kvstore/database.h"

#include <gtest/gtest.h>

// Test fixture with a empty Database object
class DatabaseTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  kvstore::DataBase db_;
};

// Putting only one key value pair
// Should return true
TEST_F(DatabaseTest, BasicPut) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
}

// Putting two entries with same key but different values
// Should return true
TEST_F(DatabaseTest, PutSameKeyDifferntValue) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value2"));
}

// Putting two entries with same key and same value
// Should return false
TEST_F(DatabaseTest, PutSameKeyValueTwice) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_FALSE(db_.PutIntoStorage("key1", "value1"));
}

// Getting the value from existing key
// Should return true
TEST_F(DatabaseTest, GetCorrectKey) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_TRUE(value_opt);
  ASSERT_EQ((*value_opt)[0], "value1");
}

// Getting the value from non-existing key
// Should return false
TEST_F(DatabaseTest, GetWrongKey) {
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_FALSE(value_opt);
}

// Removing correct key
// Should return true
TEST_F(DatabaseTest, RemoveCorrectKey) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_TRUE(db_.RemoveFromStorage("key1"));

  // Should not be able to get key
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_FALSE(value_opt);
}

// Removing wrong key
// Should return false
TEST_F(DatabaseTest, RemoveWrongKey) {
  ASSERT_FALSE(db_.RemoveFromStorage("key1"));
}
