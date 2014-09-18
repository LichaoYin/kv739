#include "kv_client.h"
#include <string>

KVClient::KVClient(std::string server_addr, int server_port) 
    : server_addr_(server_addr),
      server_port_(server_port) {
}

bool KVClient::Init() {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_ < 0) {
    perror("[ERROR]: failed on opening socket");
    return false;
  }

  bzero(&address_, sizeof(address_));
  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = htons(INADDR_ANY);
  address_.sin_port = htons(0);
   
  if (bind(socket_, (sockaddr*) &address_, sizeof(address_))) {
    perror("[ERROR]: failed on binding socket");
    return false;
  }

  bzero(&server_address_, sizeof(server_address_));
  server_address_.sin_family = AF_INET;
  if (inet_aton(server_addr_.c_str(), &server_address_.sin_addr) == 0) {
    perror("ERROR on server's IP");
    exit(1);
  }
  server_address_.sin_port = htons(server_port_);

  return true;
}

bool KVClient::ConnectServer() {
  sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  if (inet_aton(server_addr_.c_str(), &server_addr.sin_addr) == 0) {
    perror("ERROR on server's IP");
    exit(1);
  }
  server_addr.sin_port = htons(server_port_);
  socklen_t server_addr_length = sizeof(server_addr);
  if (connect(socket_, (sockaddr*) &server_addr, server_addr_length) < 0) {
    perror("[ERROR]: failed on connecting server");
    return false;
  }

  return true;
}

void KVClient::Close() {
  int rc = shutdown(socket_, 2);
  //close(socket_);
  printf("%d\n", rc);
}

void KVClient::Send(KVMessage& msg) {
  std::string msg_str;
  msg.SerializeToString(&msg_str);
  socklen_t server_addr_length = sizeof(server_address_);
  printf("%s\n", inet_ntoa(server_address_.sin_addr));
  int len;
  while (1) {
    len = sendto(socket_, msg_str.data(), msg_str.length(), 0,
                 (sockaddr*) &server_address_, server_addr_length);
    if (len >0) 
      break;
  }
  printf("here\n");
}

//void KVClient::Send(KVMessage& msg) {
//  std::string msg_str;
//  msg.SerializeToString(&msg_str);
//  write(socket_, msg_str.data(), msg_str.length());
//}

//KVMessage KVClient::Receive() {
//  char message_buffer[4096];
//  read(socket_, message_buffer, 4096);
//  KVMessage msg;
//  msg.ParseFromString(std::string(message_buffer));
//  return msg;
//}

KVMessage KVClient::Receive() {
  char message_buffer[4096];
  socklen_t len = sizeof(server_address_);
  recvfrom(socket_, message_buffer, 4096, 0, (sockaddr*) &server_address_,
           &len);
  KVMessage msg;
  msg.ParseFromString(std::string(message_buffer));
  return msg;
}
