#include "database.h"

#include <gtest/gtest.h>

// Test of Database Put and Get functionalities
TEST(DatabaseTest, PutAndGet) {
  {
    // Putting two entries with the same key
    // Should not allow the second put
    DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.PutIntoStorage("key1", "value2"));
  }

  {
    // Putting and getting with the same key
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_EQ(db.GetFromStorage("key1", &success), "value1");
    EXPECT_TRUE(success);
  }

  {
    // Putting and getting with a different key
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_NE(db.GetFromStorage("key2", &success), "value1");
    EXPECT_FALSE(success);
  }
}

// Test of Database Put and Remove functionalities
TEST(DatabaseTest, PutAndRemove) {
  {
    // Putting and removing with the same key
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_TRUE(db.RemoveFromStorage("key1"));

    EXPECT_NE(db.GetFromStorage("key1", &success), "value1");
    EXPECT_FALSE(success);
  }

  {
    // Putting and removing with a different key
    DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.RemoveFromStorage("key2"));
  }
}