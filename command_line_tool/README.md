# Command-line Tool

## Description
This is the user interface of the Warble service. It is responsible for reading input from command-line flags and communicating with Func server.

## Code Structure
1. `main.cc` creates a Func client and passes it into CommandLineTool class.
2. `command_line_tool.h/cc` is a class containing handlers for each type of command. If it finds a valid handler, it will let the handler do the rest.
3. `command_handler.h/cc` consists of a base class for all handlers and the implementation of handlers. Each handler is responsible for communicating with Func server through a Func client. All messages are sent using protobuf.
