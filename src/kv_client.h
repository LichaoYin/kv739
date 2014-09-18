#ifndef KV_CLIENT_H
#define KV_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "proto/message.pb.h"

class KVClient {
 public:
  KVClient(std::string server_addr, int server_port);
  bool Init();
  bool ConnectServer();
  void Send(KVMessage& msg);
  KVMessage Receive();
  void Close();

 private:
  sockaddr_in address_;
  sockaddr_in server_address_;
  int socket_;

  std::string server_addr_;
  int server_port_;
};

#endif
