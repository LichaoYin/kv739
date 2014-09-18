DIR=$(HOME)/Documents/UW-Madison/CS_739/Proj_1/
CC=g++
CFLAGS=-c -Wall -O2 -fPIC
LDFLAGS=-L $(DIR)/protobuf/lib -lprotobuf
INCLUDE=-I $(DIR)/protobuf/include

all:message

message: message.pb.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) message.pb.cc

clean:
	rm -rf *.o
