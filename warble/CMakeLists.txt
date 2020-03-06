cmake_minimum_required(VERSION 3.5.1)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Proto file path
get_filename_component(warble_proto "../protos/warble.proto" ABSOLUTE)
get_filename_component(warble_proto_path "${warble_proto}" PATH)

# warble
set(warble_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/warble.pb.cc")
set(warble_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/warble.pb.h")
add_custom_command(
      OUTPUT "${warble_proto_srcs}" "${warble_proto_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${warble_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${warble_proto}"
      DEPENDS "${warble_proto}")

# Compiles warble_func as a library
add_library(warble_func STATIC
  warble_func.cc warble_func.h
  ${warble_proto_srcs})
target_link_libraries(warble_func
  ${_PROTOBUF_LIBPROTOBUF}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  kvstore_client)

# Compiles warble proto as a library
add_library(warble_proto_lib ${warble_proto_srcs})
target_link_libraries(warble_proto_lib ${_PROTOBUF_LIBPROTOBUF})

# Google test
add_executable(warble_func_test warble_func_test.cc)
target_link_libraries(warble_func_test
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  warble_func
  gtest gtest_main pthread)

# add_custom_target(run ALL
#                   DEPENDS ${warble_proto_srcs}
#                           ${warble_proto_hdrs})