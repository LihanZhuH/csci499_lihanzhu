#include "database.h"

#include <gtest/gtest.h>

// Test of Database Put and Get functionalities
TEST(DatabaseTest, PutAndGet) {
  {
    // Putting two entries with the same key
    // Should not allow the second put
    kvstore::DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.PutIntoStorage("key1", "value2"));
  }

  {
    // Putting and getting with the same key
    kvstore::DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    auto value_opt = db.GetFromStorage("key1");
    ASSERT_TRUE(value_opt);
    EXPECT_EQ((*value_opt)[0], "value1");
  }

  {
    // Putting and getting with a different key
    kvstore::DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    auto value_opt = db.GetFromStorage("key2");
    ASSERT_FALSE(value_opt);
  }
}

// Test of Database Put and Remove functionalities
TEST(DatabaseTest, PutAndRemove) {
  {
    // Putting and removing with the same key
    kvstore::DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_TRUE(db.RemoveFromStorage("key1"));

    auto value_opt = db.GetFromStorage("key1");
    ASSERT_FALSE(value_opt);
  }

  {
    // Putting and removing with a different key
    kvstore::DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.RemoveFromStorage("key2"));
  }
}