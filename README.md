# CSCI 499 Project

## Project Dependencies
All of the following need to be installed globally.

1. gRPC
2. Protobuf
3. CMake
4. Google Test
5. glog
6. gflags

## Build Instruction

1. In root directory, run:
```
cmake .
cmake --build .
```
2. All executables are generated in `bin` folder.

## Steps to Run

1. Start `bin/func_server` and `bin/kvstore_server`.
2. Run command line user interface `bin/user_interface` (See help with `bin/user_interface --help`).