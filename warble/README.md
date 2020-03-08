# Warble Functionality

## Description
This is a collection of functions to be called by Func infrastructure. The functions are:

- Registeruser(username)
- Warble(username, text, (reply to warble id))
- Follow(username, user to follow)
- Read(warble id)
- Profile()

## Code Structure
1. `warble_func.h/cc` contains all the Warble functions and a `KVStoreClientAbstract` pointer. When called by Func infrastructure, it validates the request and sends reply. Meanwhile, it reads and gets from the Key-Value Store server.
2. `warble_func_test.cc` contains unit tests of `WarbleFunc` class using a `KVStoreTestClient` to emulate backend storage.
