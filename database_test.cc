#include "database.h"

#include <gtest/gtest.h>

TEST(DatabaseTest, PutAndGet) {
  {
    // Putting two entries with the same key
    DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.PutIntoStorage("key1", "value2"));
  }

  {
    // Putting and getting
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_EQ(db.GetFromStorage("key1", &success), "value1");
    EXPECT_TRUE(success);
  }

  {
    // Putting and getting
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_NE(db.GetFromStorage("key2", &success), "value1");
    EXPECT_FALSE(success);
  }
}

TEST(DatabaseTest, PutAndRemove) {
  {
    // Putting and removing
    DataBase db;
    bool success;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_TRUE(db.RemoveFromStorage("key1"));

    EXPECT_NE(db.GetFromStorage("key1", &success), "value1");
    EXPECT_FALSE(success);
  }

  {
    // Putting and removing
    DataBase db;
    EXPECT_TRUE(db.PutIntoStorage("key1", "value1"));
    EXPECT_FALSE(db.RemoveFromStorage("key2"));
  }
}