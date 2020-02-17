#include "event_handler.h"

#include <memory>

#include <gtest/gtest.h>

#include "kvstore/kvstore_client.h"

// Test of Hook
TEST(EventHandlerTest, Hook) {
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Hook(2, "warble"));
  }
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_FALSE(event_handler.Hook(1, "warble"));  // Event type is used
  }
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_FALSE(event_handler.Hook(1, "random"));  // Invalid function name
  }
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_FALSE(event_handler.Hook(1, "random"));  // Invalid function name
    EXPECT_TRUE(event_handler.Hook(1, "warble"));
  }
}

// Test of Unhook
TEST(EventHandlerTest, Unhook) {
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Unhook(1));
  }
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_FALSE(event_handler.Unhook(2));  // Does not exist
  }
  {
    func::EventHandler event_handler(std::make_shared<kvstore::KVStoreTestClient>());
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Unhook(1));
    EXPECT_FALSE(event_handler.Unhook(1));  // Already removed
  }
}