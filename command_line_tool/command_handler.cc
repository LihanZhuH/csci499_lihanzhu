#include "command_line_tool/command_handler.h"

namespace command_line {

// Define all flags
DEFINE_string(registeruser, "", "Registers the given username.");
DEFINE_string(user, "", "Logs in as the given username.");
DEFINE_string(warble, "", "Creates a new warble with the given text.");
DEFINE_string(reply, "",
    "Indicates that the new warble is a reply to the given id.");
DEFINE_string(follow, "", "Starts following the given username.");
DEFINE_string(read, "", "Reads the warble thread starting at the given id.");
DEFINE_bool(profile, false,
    "Gets the user’s profile of following and followers.");

// -- Implementation of Handler --

bool Handler::HookEventFunction(int event_type, std::string event_function) {
  return func_client_->Hook(event_type, event_function);
}

// -- Implementation of RegisterHandler --

bool RegisterHandler::IsValidCommand() {
  if (FLAGS_registeruser.empty()) {
    return false;
  }
  if (!FLAGS_user.empty() || !FLAGS_warble.empty() ||
      !FLAGS_reply.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool RegisterHandler::ProcessCommand() {
  // Hook event
  HookEventFunction(kRegisterEventType, kRegisterEventFunction);

  // Send request
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_registeruser);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kRegisterEventType, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Successfully registered!" << std::endl;
    return true;
  }
  std::cout << "Failed to register." << std::endl;
  LOG(WARNING) << "CommandLine: Register command failed";
  return false;
}

// -- Implementation of WarbleHandler --

bool WarbleHandler::IsValidCommand() {
  if (FLAGS_user.empty() || FLAGS_warble.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool WarbleHandler::ProcessCommand() {
  // Hook event
  HookEventFunction(kWarbleEventType, kWarbleEventFunction);

  // Send request
  warble::WarbleRequest request;
  warble::WarbleReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_user);
  request.set_text(FLAGS_warble);
  if (!FLAGS_reply.empty()) {
    request.set_parent_id(FLAGS_reply);
  }
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kWarbleEventType, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Warble ID: " << reply.warble().id() << std::endl;
    std::cout << "Successfully posted warble!" << std::endl;
    return true;
  }
  std::cout << "Failed to post warble." << std::endl;
  LOG(WARNING) << "CommandLine: Warble command failed";
  return false;
}

// -- Implementation of FollowHandler --

bool FollowHandler::IsValidCommand() {
  if (FLAGS_user.empty() || FLAGS_follow.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool FollowHandler::ProcessCommand() {
  // Hook event
  HookEventFunction(kFollowEventType, kFollowEventFunction);

  // Send request
  warble::FollowRequest request;
  warble::FollowReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_user);
  request.set_to_follow(FLAGS_follow);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kFollowEventType, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Successfully followed!" << std::endl;
    return true;
  }
  std::cout << "Failed to follow." << std::endl;
  LOG(WARNING) << "CommandLine: Follow command failed";
  return false;
}

// -- Implementation of ReadHandler --

bool ReadHandler::IsValidCommand() {
  if (FLAGS_user.empty() || FLAGS_read.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool ReadHandler::ProcessCommand() {
  // Hook event
  HookEventFunction(kReadEventType, kReadEventFunction);

  // Send request
  warble::ReadRequest request;
  warble::ReadReply reply;
  google::protobuf::Any request_payload;
  request.set_warble_id(FLAGS_read);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kReadEventType, request_payload)) {
    optional_payload->UnpackTo(&reply);
    // Display all warbles
    auto repeated_warbles = reply.warbles();
    for (int i = 0; i < repeated_warbles.size(); ++i) {
      auto warble = repeated_warbles.at(i);
      auto timestamp = warble.timestamp();
      std::cout << "Username: " << warble.username() << std::endl
                << "Time: " << timestamp.seconds() << std::endl
                << "Warble ID: " << warble.id() << std::endl
                << "Warble text: " << warble.text() << std::endl;
      if (!warble.parent_id().empty()) {
        std::cout << "Parent warble ID: " << warble.parent_id() << std::endl;
      }
      std::cout << std::endl;
    }
    return true;
  }
  std::cout << "Failed to read warble." << std::endl;
  LOG(WARNING) << "CommandLine: Read command failed";
  return false;
}

// -- Implementation of ProfileHandler --

bool ProfileHandler::IsValidCommand() {
  if (FLAGS_user.empty() || !FLAGS_profile) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty()) {
    return false;
  }
  return true;
}

bool ProfileHandler::ProcessCommand() {
  // Hook event
  HookEventFunction(kProfileEventType, kProfileEventFunction);

  // Send request
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_user);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kProfileEventType, request_payload)) {
    optional_payload->UnpackTo(&reply);
    // Display followers
    if (reply.followers_size() == 0) {
      std::cout << "Followers: none" << std::endl;
    } else {
      auto repeated_followers = reply.followers();
      std::cout << "Followers:" << std::endl;
      for (int i = 0; i < repeated_followers.size(); ++i) {
        std::cout << '\t' << repeated_followers.at(i) << std::endl;
      }
    }
    // Display followings
    if (reply.following_size() == 0) {
      std::cout << "Followings: none" << std::endl;
    } else {
      auto repeated_followings = reply.following();
      std::cout << "Followings:" << std::endl;
      for (int i = 0; i < repeated_followings.size(); ++i) {
        std::cout << '\t' << repeated_followings.at(i) << std::endl;
      }
    }
    return true;
  }
  std::cout << "Failed to display profile." << std::endl;
  LOG(WARNING) << "CommandLine: Profile command failed";
  return false;
}

}  // namespace command_line