#include <string>
#include <iostream>

#include <gflags/gflags.h>

DEFINE_string(registeruser, "", "Registers the given username.");
DEFINE_string(user, "", "Logs in as the given username.");
DEFINE_string(warble, "", "Creates a new warble with the given text.");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id.");
DEFINE_string(follow, "", "Starts following the given username.");
DEFINE_string(read, "", "Reads the warble thread starting at the given id.");
DEFINE_string(profile, "", "Gets the user’s profile of following and followers.");

int main(int argc, char *argv[]) {
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_profile != "") {
    std::cout << FLAGS_profile << std::endl;
  }
  return 0;
}
