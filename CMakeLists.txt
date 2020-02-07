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

# Proto file
get_filename_component(kv_proto "./protos/kvstore.proto" ABSOLUTE)
get_filename_component(kv_proto_path "${kv_proto}" PATH)
get_filename_component(func_proto "./protos/func.proto" ABSOLUTE)
get_filename_component(func_proto_path "${func_proto}" PATH)

# find glog
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GLog REQUIRED)

# find gtest
find_package(GTest REQUIRED)

# Generated sources
# kvstore
set(kv_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.pb.cc")
set(kv_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.pb.h")
set(kv_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.grpc.pb.cc")
set(kv_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.grpc.pb.h")
add_custom_command(
      OUTPUT "${kv_proto_srcs}" "${kv_proto_hdrs}" "${kv_grpc_srcs}" "${kv_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${kv_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${kv_proto}"
      DEPENDS "${kv_proto}")

# func
set(func_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/func.pb.cc")
set(func_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/func.pb.h")
set(func_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/func.grpc.pb.cc")
set(func_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/func.grpc.pb.h")
add_custom_command(
      OUTPUT "${func_proto_srcs}" "${func_proto_hdrs}" "${func_grpc_srcs}" "${func_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${func_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${func_proto}"
      DEPENDS "${func_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Compiles database as a library
add_library(database STATIC database.cc database.h)

# Google test
add_executable(database_test database_test.cc)
# include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
# include_directories("${CMAKE_CURRENT_BINARY_DIR}/googletest/include")
target_link_libraries(database_test database gtest gtest_main "pthread")

# Generate targets
foreach(_target
  kvstore_server)
  add_executable(${_target} "${_target}.cc"
    ${kv_proto_srcs}
    ${kv_grpc_srcs})
  target_link_libraries(${_target}
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    database
    ${GLOG_LIBRARIES})
endforeach()

add_custom_target(run ALL
                  DEPENDS ${func_proto_srcs}
                          ${func_proto_hdrs}
                          ${func_grpc_srcs}
                          ${func_grpc_hdrs})