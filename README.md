# CSCI 499 Project

### *Author: Lihan Zhu*

## Environment Setup

### Setting up Vagrant
1. Install Vagrant. https://www.vagrantup.com/downloads.html
2. Install VirtualBox. https://www.virtualbox.org/wiki/Downloads
3. Create a directory and clone this repository.
```
$ mkdir cs499_vagrant
$ cd cs499_vagrant
$ git clone https://github.com/LihanZhuH/csci499_lihanzhu.git
```
4. Initialize and start Vagrant box.
```
$ vagrant init ubuntu/bionic64
$ vagrant up
$ vagrant ssh
```
Now you are all set. The repository should be visible inside VM in directory `/vagrant`. The rest of this tutorial is done in VM.

### Installing Dependencies
Before installing modules, check updates and install prerequisites.
```
$ [sudo] apt-get update
$ [sudo] apt-get install build-essential autoconf libtool pkg-config
```

All of the following need to be installed globally.

1. CMake
```
$ sudo apt-get install cmake
```
2. gRPC
```
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ git submodule update --init
$ make
$ sudo make install
```
3. Protobuf
```
$ cd third_party/protobuf
$ sudo make install
```
4. Google Test
```
$ cd ../googletest
$ cmake .
$ make
$ sudo make install
```
5. gflags
```
$ cd ../gflags
$ cmake .
$ make
$ sudo make install
```
6. glog
```
$ cd /tmp
$ git clone https://github.com/google/glog.git
$ cd glog
$ cmake -H. -Bbuild -G "Unix Makefiles"
$ cmake --build build
$ sudo cmake --build build --target install
```

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