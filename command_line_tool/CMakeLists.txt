cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(command_line_tool command_line_tool.cc)
target_link_libraries(command_line_tool ${GLOG_LIBRARIES})