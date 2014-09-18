#include "kv_server.h"

#include <unordered_map>
#include <string>

KVServer::KVServer(int server_port) 
    : port_(server_port),
      database_(new KVDatabase()) {
}

bool KVServer::Init() {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);  // TCP Connection
  if (socket_ < 0) {
    perror("[ERROR] Failed on opening socket");
    return false;
  }
  bzero((char*) &address_, sizeof(address_));
  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = htons(INADDR_ANY);
  address_.sin_port = htons(port_);

  if (bind(socket_, (sockaddr*) &address_, sizeof(address_)) < 0) {
    perror("[ERROR] Failed on binding socket");
    return false;
  }

  //if (listen(socket_, num_waiting_queue_)) {
  //  perror("[ERROR] Failed on binding socket");
  //  return false;
  //}

  database_->Init();

  return true;
}

int KVServer::AcceptConnection() {
  sockaddr_in client_address;
  socklen_t length = sizeof(client_address);
  int new_server_socket = accept(socket_, (sockaddr*) &client_address, &length);
  if (new_server_socket < 0) {
    perror("[ERROR] Failed on accepting client socket");
    return -1;
  }
  printf("New request from %s\n", inet_ntoa(client_address.sin_addr));
  client_connections_.insert(std::make_pair(new_server_socket,
                                            &client_address));
  return new_server_socket;
}

bool KVServer::Receive(sockaddr_in* from, KVMessage* msg) {
  char message_buffer[4096];
  socklen_t len;
  int rbytes = recvfrom(socket_, message_buffer, sizeof(message_buffer), 0,
                        (struct sockaddr*) from, &len);
  printf("New request from %s\n", inet_ntoa(from->sin_addr));
  msg->ParseFromString(std::string(message_buffer));
  ProcessMessage(msg);
  if (rbytes > 0) 
    return true;
  else 
    return false;
}

// return true if the request needs to be replied, otherwise false.
void KVServer::Receive(int client_socket, KVMessage* msg) {
  char message_buffer[4096];
  read(client_socket, message_buffer, 4096);
  msg->ParseFromString(std::string(message_buffer));
  ProcessMessage(msg);
}

void KVServer::ProcessMessage(KVMessage* msg) {
  // The current value associated with this key.
  std::string value;
  database_->Get(msg->key(), &value);
  if (msg->has_value()) {  // Put request
    printf("here1\n");
    database_->Delete(msg->key());
    printf("here2\n");
    database_->Put(msg->key(), msg->value());
    msg->clear_value();
  }
  printf("ProcessMessage:\n%s", msg->DebugString().c_str());
  if (!value.empty()) {
    msg->set_value(value);
  }
}

void KVServer::Send(sockaddr_in& to, KVMessage& msg) {
  std::string msg_str;
  printf("Send: %s\n", msg.DebugString().c_str());
  msg.SerializeToString(&msg_str);
  sendto(socket_, msg_str.data(), msg_str.length(), 0, (sockaddr*) &to,
         sizeof(to));
}

void KVServer::Send(int client_socket, KVMessage& msg) {
  std::string msg_str;
  printf("Send: %s\n", msg.DebugString().c_str());
  msg.SerializeToString(&msg_str);
  write(client_socket, msg_str.data(), msg_str.length());
}
