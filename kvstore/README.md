# Backend Key-Value Store

## Description
Backend Key-Value Store provides put, get and remove services. Two entries with the same key is allowed. It is used by Func infrastructure to store user data.

## Code Structure
1. `kvstore_server.h/cc` is a server listening to put, get and remove requests. It uses a local `DataBase` object to hold the data.
2. `kvstore_client.h/cc` contains a base class for Key-Value Store clients, one implementation that communicates with Key-Value Store server, and another implementation that keeps a local storage for testing purposes.
3. `database.h/cc` provides thread-safe access to an unordered_map. It supports put, get and remove function calls.
4. `database_test.cc` contains unit test of `DataBase` class.
