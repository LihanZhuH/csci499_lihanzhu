#include <string>
#include <iostream>

#include <gflags/gflags.h>

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
  if (FLAGS_registeruser.length() == 0) {
    return false;
  }
  if (FLAGS_user.length() > 0 || FLAGS_warble.length() > 0 ||
      FLAGS_reply.length() > 0 || FLAGS_follow.length() > 0 ||
      FLAGS_read.length() > 0 || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for NEW_WARBLE
bool ValidWarbleCmd() {
  if (FLAGS_user.length() == 0 || FLAGS_warble.length() == 0) {
    return false;
  }
  if (FLAGS_registeruser.length() > 0 || FLAGS_reply.length() > 0 ||
      FLAGS_follow.length() > 0 || FLAGS_read.length() > 0 || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for NEW_REPLY
bool ValidReplyCmd() {
  if (FLAGS_user.length() == 0 || FLAGS_warble.length() == 0 ||
      FLAGS_reply.length() == 0) {
    return false;
  }
  if (FLAGS_registeruser.length() > 0 || FLAGS_follow.length() > 0 ||
      FLAGS_read.length() > 0 || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for FOLLOW
bool ValidFollowCmd() {
  if (FLAGS_user.length() == 0 || FLAGS_follow.length() == 0) {
    return false;
  }
  if (FLAGS_registeruser.length() > 0 || FLAGS_reply.length() > 0 ||
      FLAGS_warble.length() > 0 || FLAGS_read.length() > 0 || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for READ_WARBLE
bool ValidReadCmd() {
  if (FLAGS_user.length() == 0 || FLAGS_read.length() == 0) {
    return false;
  }
  if (FLAGS_registeruser.length() > 0 || FLAGS_reply.length() > 0 ||
      FLAGS_warble.length() > 0 || FLAGS_follow.length() > 0 || FLAGS_profile) {
    return false;
  }
  return true;
}

// Return true if flags are for READ_WARBLE
bool ValidProfileCmd() {
  if (FLAGS_user.length() == 0 || !FLAGS_profile) {
    return false;
  }
  if (FLAGS_registeruser.length() > 0 || FLAGS_reply.length() > 0 ||
      FLAGS_warble.length() > 0 || FLAGS_follow.length() > 0 ||
      FLAGS_read.length() > 0) {
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
    std::cout << "REGISTER" << std::endl;
    return 0;
  } else if (command_line::ValidWarbleCmd()) {
    std::cout << "NEW_WARBLE" << std::endl;
    return 0;
  } else if (command_line::ValidReplyCmd()) {
    std::cout << "NEW_REPLY" << std::endl;
    return 0;
  } else if (command_line::ValidFollowCmd()) {
    std::cout << "FOLLOW" << std::endl;
    return 0;
  } else if (command_line::ValidReadCmd()) {
    std::cout << "READ_WARBLE" << std::endl;
    return 0;
  } else if (command_line::ValidProfileCmd()) {
    std::cout << "PROFILE" << std::endl;
    return 0;
  } else {
    std::cout << "INVALID OPTION" << std::endl;
    return 1;
  }
  return 1;
}
