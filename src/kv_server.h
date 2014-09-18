#ifndef KV_SERVER_H
#define KV_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>
#include <memory>

#include "kv_db.h"
#include "proto/message.pb.h"

class KVServer {
 public:
  explicit KVServer(int server_port);
  bool Init();
  int AcceptConnection();
  void Receive(int client_socket, KVMessage* msg);
  bool Receive(sockaddr_in* from, KVMessage* msg);
  void ProcessMessage(KVMessage* msg);
  void Send(int client_socket, KVMessage& msg);
  void Send(sockaddr_in& to, KVMessage& msg);

 private:
  int socket_;
  int port_;
  const int num_waiting_queue_ = 5;
  sockaddr_in address_;

  std::unordered_map<int, sockaddr_in*> client_connections_;
  std::unique_ptr<KVDatabase> database_;
};

#endif
