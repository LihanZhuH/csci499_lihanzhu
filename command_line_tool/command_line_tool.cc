#include <string>
#include <iostream>

#include <gflags/gflags.h>
#include <glog/logging.h>

namespace command_line {

// Define all flags
DEFINE_string(registeruser, "", "Registers the given username.");
DEFINE_string(user, "", "Logs in as the given username.");
DEFINE_string(warble, "", "Creates a new warble with the given text.");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id.");
DEFINE_string(follow, "", "Starts following the given username.");
DEFINE_string(read, "", "Reads the warble thread starting at the given id.");
DEFINE_bool(profile, false, "Gets the user’s profile of following and followers.");

// Return true if flags are for REGISTER
bool ValidRegCmd() {
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

// Return true if flags are for NEW_WARBLE
bool ValidWarbleCmd() {
  if (FLAGS_user.empty() || FLAGS_warble.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_follow.empty() || !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for NEW_REPLY
bool ValidReplyCmd() {
  if (FLAGS_user.empty() || FLAGS_warble.empty() ||
      FLAGS_reply.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_follow.empty() ||
      !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for FOLLOW
bool ValidFollowCmd() {
  if (FLAGS_user.empty() || FLAGS_follow.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_read.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for READ_WARBLE
bool ValidReadCmd() {
  if (FLAGS_user.empty() || FLAGS_read.empty()) {
    return false;
  }
  if (!FLAGS_registeruser.empty() || !FLAGS_reply.empty() ||
      !FLAGS_warble.empty() || !FLAGS_follow.empty() || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for READ_WARBLE
bool ValidProfileCmd() {
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

}  // command_line

int main(int argc, char *argv[]) {
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  // Check each combination of arguments
  // TODO: Will call different function and send data to Func server
  if (command_line::ValidRegCmd()) {
    LOG(INFO) << "CommandLine: Received REGISTER";
    return 0;
  } else if (command_line::ValidWarbleCmd()) {
    LOG(INFO) << "CommandLine: Received NEW_WARBLE";
    return 0;
  } else if (command_line::ValidReplyCmd()) {
    LOG(INFO) << "CommandLine: Received NEW_REPLY";
    return 0;
  } else if (command_line::ValidFollowCmd()) {
    LOG(INFO) << "CommandLine: Received FOLLOW";
    return 0;
  } else if (command_line::ValidReadCmd()) {
    LOG(INFO) << "CommandLine: Received READ_WARBLE";
    return 0;
  } else if (command_line::ValidProfileCmd()) {
    LOG(INFO) << "CommandLine: Received PROFILE";
    return 0;
  } else {
    LOG(WARNING) << "CommandLine: Received INVALID OPTION";
    return 1;
  }
  return 1;
}
