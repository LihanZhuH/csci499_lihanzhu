cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Proto file path
get_filename_component(kv_proto "../protos/kvstore.proto" ABSOLUTE)
get_filename_component(kv_proto_path "${kv_proto}" PATH)

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

# Compiles database as a library
add_library(database STATIC database.cc database.h)

# Google test
add_executable(database_test database_test.cc)
target_link_libraries(database_test database gtest gtest_main "pthread")

# Generate executables
add_executable(kvstore_server "kvstore_server.cc"
  ${kv_proto_srcs}
  ${kv_grpc_srcs})
target_link_libraries(kvstore_server
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  database
  ${GLOG_LIBRARIES})