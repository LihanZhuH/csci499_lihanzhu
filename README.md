# CSCI 499 Project

### *Author: Lihan Zhu*

## Environment Setup
You will need a linux environment (e.g. Ubuntu 18.04.) with all of the following installed globally:

1. CMake
2. gRPC
3. Protobuf
4. Google Test
5. gflags
6. glog

For installation, please follow [this guide](SETUP.md).

## Build Instruction

1. In root directory of `csci499_lihanzhu`, run:
```
$ cmake .
$ cmake --build .
```
2. All executables will be generated in `bin` folder, including unit tests.

## Steps to Run
1. Go to `bin` folder where all executables are generated.
```
$ cd bin
```

2. Always start `func_server` and `kvstore_server` first. In two different terminals, run the servers.
```
$ ./func_server
```
```
$ ./kvstore_server
```
The servers should start listening to requests.

3. In another terminal, run command line user interface `user_interface` (See help with `user_interface --help`).
```
$ ./user_interface <flag> <argument> ... 
```

### Examples:
Functionalities | Commands
--- | ---
Register a new user | `$ ./user_interface --registeruser "New User"`
Post a new warble | `$ ./user_interface --user "Existing User" --warble "Hello warble!"`
Read a warble | `$ ./user_interface --user "Existing User" --read 1`
Reply to a warble | `$ ./user_interface --user "Existing User" --warble "Reply." --reply 1`
Follow another user | `$ ./user_interface --user "Existing User" --follow "Another Existing User"`
See profile | `$ ./user_interface --user "Existing User" --profile`

## Running Tests

There are three unit tests generated in `bin` folder:

1. `data_base_test` for backend Key-Value Store implementation.
2. `event_handler_test` for Func infrastructure implementation.
3. `warble_func_test` for Warble functionality.
