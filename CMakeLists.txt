cmake_minimum_required(VERSION 3.5.1)

project(HelloWorld C CXX)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Threads REQUIRED)

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

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
# find_package(gRPC REQUIRED)
# message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)

# Proto file
get_filename_component(kv_proto "./protos/kvstore.proto" ABSOLUTE)
get_filename_component(kv_proto_path "${kv_proto}" PATH)

# Generated sources
set(kv_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.pb.cc")
set(kv_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.pb.h")
set(kv_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kvstore.grpc.pb.h")
add_custom_command(
      OUTPUT "${kv_proto_srcs}" "${kv_proto_hdrs}" "${kv_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${kv_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${kv_proto}"
      DEPENDS "${kv_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Compiles database as a library
add_library(database STATIC database.cc database.h)

add_executable(database_test database_test.cc)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/googletest/include")

target_link_libraries(database_test database gtest gtest_main "gtest" "gtest_main" "pthread")

# add_library(gtest_main STATIC gtest_main.cc)

# # Adds test
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# set(GTEST_BOTH_LIBRARIES "")

# add_executable(database_test database_test.cc)

# target_link_libraries(database_test ${GTEST_BOTH_LIBRARIES})

# add_test(AllTestsInFoo database_test)

# gtest_add_tests(database_test "" AUTO)
# target_link_libraries(database_test PUBLIC
#     gtest
#     gtest_main
#     database
# )
# add_test(
#     NAME database_test
#     COMMAND database_test
# )

# Targets greeter_[async_](client|server)
foreach(_target
  kvstore_server)
  add_executable(${_target} "${_target}.cc"
    ${kv_proto_srcs}
    ${kv_grpc_srcs})
  target_link_libraries(${_target}
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF}
    database)
endforeach()
