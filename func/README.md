# Func FaaS infrastructure

## Description
The Func Faas system supports hook, unhook and event functions. As the central piece of this software system, it listens to requests from Func clients and uses the Key-Value Store services to keep data.

## Code Structure
1. `func_server.h/cc` is the server listening to hook, unhook and event requests and hands over requests to `EventHandler` class. It listens on port 50000.
2. `func_client.h/cc` contains a base class for Func clients and the implemtation class that communicates with Func server. It keeps a pointer of stub to Func server. Given request parameters, a Func client will put them into a protobuf and send it to server.
3. `event_handler.h/cc` is the class that contains all the logic of the Func server. When called by Func server with a request, it calls the corresponding Warble function.
4. `event_handler_test.cc` contains unit tests of `EventHandler` class using a `KVStoreTestClient` to emulate backend storage.
