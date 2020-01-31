HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl -pthread
endif

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ./protos

vpath %.proto $(PROTOS_PATH)

all: kvstore.grpc.pb.cc kvstore.pb.cc kvstore_server kvstore_server_test func.grpc.pb.cc func.pb.cc

# kvstore
kvstore.grpc.pb.cc: $(PROTOS_PATH)/kvstore.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

kvstore.pb.cc: $(PROTOS_PATH)/kvstore.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

kvstore_server: kvstore_server.o kvstore.grpc.pb.o kvstore.pb.o
	$(CXX) $^ $(LDFLAGS) -o $@

kvstore_server_test: kvstore_server_test.o kvstore.grpc.pb.o kvstore.pb.o
	$(CXX) $^ $(LDFLAGS) -o $@

kvstore.grpc.pb.o: kvstore.grpc.pb.cc
	$(CXX) $^ $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@

kvstore.pb.o: kvstore.pb.cc
	$(CXX) $^ $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@

kvstore_server.o: kvstore_server.cc kvstore_server.h
	$(CXX) $< $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@

kvstore_server_test.o: kvstore_server_test.cc
	$(CXX) $^ $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@

# func
func.grpc.pb.cc: $(PROTOS_PATH)/func.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

func.pb.cc: $(PROTOS_PATH)/func.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

func.grpc.pb.o: func.grpc.pb.cc
	$(CXX) $^ $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@

func.pb.o: func.pb.cc
	$(CXX) $^ $(CXXFLAGS) `pkg-config --cflags protobuf grpc` -c -o $@
	
.PHONY: clean
clean:
	rm *.o *.pb.cc *.pb.h kvstore_server kvstore_server_test


# .PRECIOUS: %.grpc.pb.cc
# %.grpc.pb.cc: %.proto
# 	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

# .PRECIOUS: %.pb.cc
# %.pb.cc: %.proto
# 	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<