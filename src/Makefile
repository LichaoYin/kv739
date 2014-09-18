PROTOBUF_DIR=$(HOME)/Documents/UW-Madison/CS_739/Proj_1/protobuf
CC=g++
CFLAGS=-Wall -O2 -std=c++11
LDFLAGS=-L$(PROTOBUF_DIR)/lib -lprotobuf
INCLUDE=-I$(PROTOBUF_DIR)/include

#all: server

all: 
	make -C proto
	$(CC) -c -lsqlite3 kv_db.cc
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(INCLUDE) kv_db.o kv_server.cc
	$(CC) -c -fPIC $(CFLAGS) $(LDFLAGS) $(INCLUDE) kv_client.cc
	$(CC) -fPIC $(CFLAGS) $(LDFLAGS) $(INCLUDE) kv739.cc kv_client.o proto/message.pb.o -shared -o lib739kv.so
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 $(INCLUDE) server.cc proto/message.pb.o kv_db.o kv_server.o -o server
	$(CC) $(CFLAGS) -L./ -l739kv client.cc -o client

clean:
	rm -rf *.o client server
