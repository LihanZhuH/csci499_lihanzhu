#include "kvstore/database.h"

#include <gtest/gtest.h>

#include <sstream>

// Test fixture with a empty Database object
class DatabaseTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  kvstore::DataBase db_;
};

// Putting only one key value pair
// Should return true
TEST_F(DatabaseTest, PutOneValueForAKeyShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
}

// Putting two entries with same key but different values
// Should return true
TEST_F(DatabaseTest, PutDifferntValueForSameKeyShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value2"));
}

// Putting two entries with same key and same value
// Should return false
TEST_F(DatabaseTest, PutSameKeyValueTwiceShouldFail) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_FALSE(db_.PutIntoStorage("key1", "value1"));
}

// Getting the value from existing key
// Should return true
TEST_F(DatabaseTest, GetExistingKeyShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_TRUE(value_opt);
  ASSERT_EQ((*value_opt)[0], "value1");
}

// Getting the value from non-existing key
// Should return false
TEST_F(DatabaseTest, GetNonExistingKeyShouldFail) {
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_FALSE(value_opt);
}

// Removing correct key
// Should return true
TEST_F(DatabaseTest, RemoveExistingKeyShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_TRUE(db_.RemoveFromStorage("key1"));

  // Should not be able to get key
  auto value_opt = db_.GetFromStorage("key1");
  ASSERT_FALSE(value_opt);
}

// Removing wrong key
// Should return false
TEST_F(DatabaseTest, RemoveNonExistingKeyShouldFail) {
  ASSERT_FALSE(db_.RemoveFromStorage("key1"));
}

// Serialize database with one entry
TEST_F(DatabaseTest, SerializeShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  std::stringstream ss;
  db_.Serialize(ss);
  EXPECT_EQ(ss.str(), "key1\n1\nvalue1\n");
}

// Serialize database with one entry
TEST_F(DatabaseTest, SerializeComplexShouldSucceed) {
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value1"));
  ASSERT_TRUE(db_.PutIntoStorage("key1", "value2"));
  ASSERT_TRUE(db_.PutIntoStorage("key2", "value3"));
  std::stringstream ss;
  db_.Serialize(ss);
  EXPECT_EQ(ss.str(), "key2\n1\nvalue3\nkey1\n2\nvalue1\nvalue2\n");
}

// Deserialize from a proper stringstream
TEST_F(DatabaseTest, DeserializeShouldSucceed) {
  std::stringstream ss("key1\n1\nvalue1\n");
  EXPECT_TRUE(db_.Deserialize(ss));
}

// Deserialize from an improper stringstream
TEST_F(DatabaseTest, DeserializeShouldFail) {
  std::stringstream ss("key1\nvalue1\n");
  EXPECT_FALSE(db_.Deserialize(ss));
}