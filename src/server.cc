#include <iostream>

#include "kv_server.h"
#include "proto/message.pb.h"

int main() {
  KVServer kv_server(6666);
  if (!kv_server.Init()) {
    perror("[Error] failed on init");
    exit(1);
  }
  
  while(true) {
    // int client_fd = kv_server.AcceptConnection();
    KVMessage msg;
    sockaddr_in client;
    if (kv_server.Receive(&client, &msg)) {
      kv_server.Send(client, msg);
    }
    //if (client_fd >= 0) {
    //  KVMessage msg;
    //  kv_server.Receive(client_fd, &msg);
    //  kv_server.Send(client_fd, msg);
    //}
  }
}
