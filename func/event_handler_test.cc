#include "func/event_handler.h"

#include <gtest/gtest.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "kvstore/kvstore_client.h"

// Test fixture with empty kvstore_server backend
// No event type is hooked
class EventHandlerBasicTest : public ::testing::Test {
 protected:
  void SetUp() override {
    client_ = std::make_shared<kvstore::KVStoreTestClient>();
    event_handler_ = std::make_shared<func::EventHandler>(client_);
  }

  std::shared_ptr<func::EventHandler> event_handler_;
  std::shared_ptr<kvstore::KVStoreTestClient> client_;
};

// Hooking a new function
// Should return true
TEST_F(EventHandlerBasicTest, BasicHook) {
  EXPECT_TRUE(event_handler_->Hook(1, "registeruser"));
  EXPECT_TRUE(event_handler_->Hook(2, "warble"));
}

// Hooking invalid function name
// Should return false
TEST_F(EventHandlerBasicTest, HookInvalidFunction) {
  EXPECT_FALSE(event_handler_->Hook(1, "random"));
}

// Unhooking an unhooked event
// Should return false
TEST_F(EventHandlerBasicTest, UnhookUnknownEvent) {
  EXPECT_FALSE(event_handler_->Unhook(1));
}

// Event with unhooked event type
// Should return false
TEST_F(EventHandlerBasicTest, EventCallOnInvalidFunction) {
  google::protobuf::Any payload;
  EXPECT_FALSE(event_handler_->Event(1, payload));
}

// Test fixture with populated kvstore_server backend
// One event type is hooked on EventHandler
class EventHandlerComplexTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::unordered_map<std::string, std::vector<std::string>> storage =
        {{"U:TEST_0", {""}},
         {"U:TEST_00", {""}}};
    client_ = std::make_shared<kvstore::KVStoreTestClient>(storage);
    event_handler_ = std::make_shared<func::EventHandler>(client_);
    event_handler_->Hook(1, "registeruser");
  }

  std::shared_ptr<func::EventHandler> event_handler_;
  std::shared_ptr<kvstore::KVStoreTestClient> client_;
};

// Hooking an existing event type
// Should return false
TEST_F(EventHandlerComplexTest, HookExistingEventType) {
  EXPECT_FALSE(event_handler_->Hook(1, "warble"));
}

// Hooking an existing event function
// Should return true
TEST_F(EventHandlerComplexTest, HookExistingEventFunc) {
  EXPECT_TRUE(event_handler_->Hook(2, "warble"));
}

// Unhooking an existing event type
// Should return true
TEST_F(EventHandlerComplexTest, UnhookExistingEventType) {
  EXPECT_TRUE(event_handler_->Unhook(1));
}

// Making a Register function call
// Should succeed
TEST_F(EventHandlerComplexTest, EventCall) {
  google::protobuf::Any payload;
  // Set up request
  warble::RegisteruserRequest request;
  request.set_username("TEST_1");
  payload.PackFrom(request);

  if (auto reply_payload = event_handler_->Event(1, payload)) {
    warble::RegisteruserReply reply;
    reply_payload->UnpackTo(&reply);
    SUCCEED();
  } else {
    FAIL();
  }
}

// Post a new warble using event handler
// Should succeed
TEST_F(EventHandlerComplexTest, EventHookAndWarble) {
  // Hook Warble
  ASSERT_TRUE(event_handler_->Hook(2, "warble"));

  // Post warble
  google::protobuf::Any payload;
  warble::WarbleRequest warble_request;
  warble_request.set_username("TEST_0");
  warble_request.set_text("Text");
  payload.PackFrom(warble_request);

  if (auto reply_payload = event_handler_->Event(2, payload)) {
    warble::WarbleReply reply;
    reply_payload->UnpackTo(&reply);
    SUCCEED();
    warble::Warble reply_warble = reply.warble();
    // Compare content
    EXPECT_EQ(reply_warble.username(), "TEST_0");
    EXPECT_EQ(reply_warble.id(), "1");
    EXPECT_EQ(reply_warble.text(), "Text");
  } else {
    FAIL();
  }

  // Hook Read
  ASSERT_TRUE(event_handler_->Hook(4, "read"));

  // Read warble
  warble::ReadRequest read_request;
  read_request.set_warble_id("1");
  payload.PackFrom(read_request);

  if (auto reply_payload = event_handler_->Event(4, payload)) {
    warble::ReadReply reply;
    reply_payload->UnpackTo(&reply);
    SUCCEED();
    ASSERT_EQ(reply.warbles().size(), 1);
    warble::Warble reply_warble = reply.warbles().at(0);
    // Compare content
    EXPECT_EQ(reply_warble.username(), "TEST_0");
    EXPECT_EQ(reply_warble.id(), "1");
    EXPECT_EQ(reply_warble.text(), "Text");
  } else {
    FAIL();
  }
}
