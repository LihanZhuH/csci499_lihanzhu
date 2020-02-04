CXX = g++
CXXFLAGS = -g -Wall -std=c++17
LDLIBS = -lgflags

all: command_line_tool

command_line_tool: command_line_tool.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)