CXX = g++
CXXFLAGS = -g -Wall -std=c++17
LDLIBS = -lgflags

all: warble

warble: warble.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)