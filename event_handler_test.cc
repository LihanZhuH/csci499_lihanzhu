#include "event_handler.h"

#include <gtest/gtest.h>

// Test of Hook
TEST(EventHandlerTest, Hook) {
  {
    EventHandler event_handler;
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Hook(2, "warble"));
  }
  {
    EventHandler event_handler;
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_FALSE(event_handler.Hook(1, "warble"));  // Event type is used
  }
  {
    EventHandler event_handler;
    EXPECT_FALSE(event_handler.Hook(1, "random"));  // Invalid function name
  }
  {
    EventHandler event_handler;
    EXPECT_FALSE(event_handler.Hook(1, "random"));  // Invalid function name
    EXPECT_TRUE(event_handler.Hook(1, "warble"));
  }
}

// Test of Unhook
TEST(EventHandlerTest, Unhook) {
  {
    EventHandler event_handler;
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Unhook(1));
  }
  {
    EventHandler event_handler;
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_FALSE(event_handler.Unhook(2));  // Does not exist
  }
  {
    EventHandler event_handler;
    EXPECT_TRUE(event_handler.Hook(1, "registeruser"));
    EXPECT_TRUE(event_handler.Unhook(1));
    EXPECT_FALSE(event_handler.Unhook(1));  // Already removed
  }
}