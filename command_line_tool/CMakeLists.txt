cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Compiles command_line_tool as a library
add_library(command_line_tool STATIC
  command_line_tool.cc command_line_tool.h
  ${warble_proto_srcs} ${PROJECT_SOURCE_DIR}/warble/warble.pb.cc)
target_link_libraries(command_line_tool
  func_client
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARY}
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  -pthread)

add_executable(user_interface main.cc)
target_link_libraries(user_interface
  command_line_tool
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARY}
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  -pthread)