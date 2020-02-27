cmake_minimum_required(VERSION 3.5.1)

include_directories(${PROJECT_SOURCE_DIR}/func)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Compiles command_handler as a library
add_library(command_handler STATIC
  command_handler.cc command_handler.h)
target_link_libraries(command_handler
  func_client
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARY}
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  warble_proto_lib
  -pthread
)

# Compiles command_line_tool as a library
add_library(command_line_tool STATIC
  command_line_tool.cc command_line_tool.h)
target_link_libraries(command_line_tool
  func_client command_handler
  ${GLOG_LIBRARIES}
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF})

# Add user_interface executable
add_executable(user_interface main.cc)
target_link_libraries(user_interface
  command_line_tool
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARY}
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  -pthread)