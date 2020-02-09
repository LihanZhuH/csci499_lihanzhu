cmake_minimum_required(VERSION 3.5.1)

project(Warble C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(Threads REQUIRED)

# Include Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Include gRPC
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)
message(STATUS "Using gRPC")

# Find glog
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GLog REQUIRED)

# Find gtest
find_package(GTest REQUIRED)

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Generate targets
# foreach(_target
#   kvstore_server func_server)
#   add_executable(${_target} "${_target}.cc"
#     ${kv_proto_srcs}
#     ${kv_grpc_srcs})
#   target_link_libraries(${_target}
#     ${_GRPC_GRPCPP_UNSECURE}
#     ${_PROTOBUF_LIBPROTOBUF}
#     database
#     ${GLOG_LIBRARIES})
# endforeach()

# add_custom_target(run ALL
#                   DEPENDS ${func_proto_srcs}
#                           ${func_proto_hdrs}
#                           ${func_grpc_srcs}
#                           ${func_grpc_hdrs})

# Adds subdirectories
add_subdirectory(kvstore)
add_subdirectory(func)