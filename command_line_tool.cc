#include <string>
#include <iostream>

#include <gflags/gflags.h>

// Define all flags
DEFINE_string(registeruser, "", "Registers the given username.");
DEFINE_string(user, "", "Logs in as the given username.");
DEFINE_string(warble, "", "Creates a new warble with the given text.");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id.");
DEFINE_string(follow, "", "Starts following the given username.");
DEFINE_string(read, "", "Reads the warble thread starting at the given id.");
DEFINE_bool(profile, false, "Gets the user’s profile of following and followers.");

// Return true if flags are for REGISTER
bool IsRegister() {
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
bool IsNewWarble() {
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
bool IsNewReply() {
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
bool IsFollow() {
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
bool IsReadWarble() {
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
bool IsProfile() {
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

int main(int argc, char *argv[]) {
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  // Check each combination of arguments
  // TODO: Will call different function and send data to Func server
  if (IsRegister()) {
    std::cout << "REGISTER" << std::endl;
  } else if (IsNewWarble()) {
    std::cout << "NEW_WARBLE" << std::endl;
  } else if (IsNewReply()) {
    std::cout << "NEW_REPLY" << std::endl;
  } else if (IsFollow()) {
    std::cout << "FOLLOW" << std::endl;
  } else if (IsReadWarble()) {
    std::cout << "READ_WARBLE" << std::endl;
  } else if (IsProfile()) {
    std::cout << "PROFILE" << std::endl;
  } else {
    std::cout << "INVALID OPTION" << std::endl;
  }
  return 0;
}
