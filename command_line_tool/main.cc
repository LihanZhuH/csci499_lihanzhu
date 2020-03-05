#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include "command_line_tool/command_line_tool.h"

int main(int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  // Initialize func client and pass into command line handler
  auto channel = grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials());
  std::shared_ptr<func::FuncClientAbstract> func_client
      (new func::FuncClientImpl(channel));
  command_line::CommandLineTool command_line_handler(func_client);

  command_line_handler.Run();
  return 0;
}
