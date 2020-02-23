#include "command_line_tool/command_line_tool.h"

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
DEFINE_bool(hook, false, "Hooks all warble functions.");

// -- Implementation of CommandLineHandler --

bool CommandLineHandler::Init() {
  // Hook all event functions
  if (
  func_client_->Hook(kRegisteruserPair.first, kRegisteruserPair.second) &&
  func_client_->Hook(kWarblePair.first, kWarblePair.second) &&
  func_client_->Hook(kFollowPair.first, kFollowPair.second) &&
  func_client_->Hook(kReadPair.first, kReadPair.second) &&
  func_client_->Hook(kProfilePair.first, kProfilePair.second)) {
    std::cout << "Successfully Hooked!" << std::endl;
    return true;
  }
  std::cout << "Failed to hook." << std::endl;
  LOG(WARNING) << "CommandLine: Init failed";
  return false;
}

bool CommandLineHandler::Run() {
  // Check each combination of arguments
  if (ValidHookCmd()) {
    return Init();
  } else if (ValidRegCmd()) {
    return SendRegisteruser();
  } else if (ValidWarbleCmd()) {
    return SendWarble();
  } else if (ValidFollowCmd()) {
    return SendFollow();
  } else if (ValidReadCmd()) {
    return SendRead();
  } else if (ValidProfileCmd()) {
    return SendProfile();
  } else {
    LOG(WARNING) << "CommandLine: Received INVALID OPTION";
    return false;
  }
}

bool CommandLineHandler::ValidRegCmd() {
  if (FLAGS_registeruser.empty()) {
    return false;
  }
  if (!FLAGS_user.empty() || !FLAGS_warble.empty() ||
      !FLAGS_reply.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_profile || FLAGS_hook) {
    return false;
  }
  return true;
}

bool CommandLineHandler::ValidWarbleCmd() {
  if (FLAGS_user.empty() || FLAGS_warble.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_profile || FLAGS_hook) {
    return false;
  }
  return true;
}

bool CommandLineHandler::ValidFollowCmd() {
  if (FLAGS_user.empty() || FLAGS_follow.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() || FLAGS_hook ||
      !FLAGS_warble.empty() || !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool CommandLineHandler::ValidReadCmd() {
  if (FLAGS_user.empty() || FLAGS_read.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() || FLAGS_hook ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool CommandLineHandler::ValidProfileCmd() {
  if (FLAGS_user.empty() || !FLAGS_profile) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_hook) {
    return false;
  }
  return true;
}

bool CommandLineHandler::ValidHookCmd() {
  if (!FLAGS_hook) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || !FLAGS_user.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

bool CommandLineHandler::SendRegisteruser() {
  // Send request
  warble::RegisteruserRequest request;
  warble::RegisteruserReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_registeruser);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kRegisteruserPair.first, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Successfully registered!" << std::endl;
    return true;
  }
  std::cout << "Failed to register." << std::endl;
  LOG(WARNING) << "CommandLine: Register command failed";
  return false;
}

bool CommandLineHandler::SendWarble() {
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
      func_client_->Event(kWarblePair.first, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Warble ID: " << reply.warble().id() << std::endl;
    std::cout << "Successfully posted warble!" << std::endl;
    return true;
  }
  std::cout << "Failed to post warble." << std::endl;
  LOG(WARNING) << "CommandLine: Warble command failed";
  return false;
}

bool CommandLineHandler::SendFollow() {
  // Send request
  warble::FollowRequest request;
  warble::FollowReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_user);
  request.set_to_follow(FLAGS_follow);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kFollowPair.first, request_payload)) {
    optional_payload->UnpackTo(&reply);
    std::cout << "Successfully followed!" << std::endl;
    return true;
  }
  std::cout << "Failed to follow." << std::endl;
  LOG(WARNING) << "CommandLine: Follow command failed";
  return false;
}

bool CommandLineHandler::SendRead() {
  // Send request
  warble::ReadRequest request;
  warble::ReadReply reply;
  google::protobuf::Any request_payload;
  request.set_warble_id(FLAGS_read);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kReadPair.first, request_payload)) {
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

bool CommandLineHandler::SendProfile() {
  // Send request
  warble::ProfileRequest request;
  warble::ProfileReply reply;
  google::protobuf::Any request_payload;
  request.set_username(FLAGS_user);
  request_payload.PackFrom(request);
  // Check reply
  if (auto optional_payload =
      func_client_->Event(kProfilePair.first, request_payload)) {
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
