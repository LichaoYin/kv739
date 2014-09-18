#include "kv739.h"

#include <string>
#include <memory>

#include "kv_client.h"

namespace {
std::unique_ptr<KVClient> kv_client;
}

int kv739_init(char* server) {
  std::string addr_port(server);
  int pos =  addr_port.find_first_of(':');
  std::string addr = addr_port.substr(0, pos);
  int port = std::stoi(addr_port.substr(pos+1));

  kv_client.reset(new KVClient(addr, port));
  if (kv_client->Init()) {
    return 0;
    //if (kv_client->ConnectServer()) {
    //  return 0;
    //}
  }
  return -1;
}

int kv739_get(char* key, char* value) {
  try {
    //if (kv_client->ConnectServer()) {
    //  return -1;
    //}
    KVMessage msg;
    msg.set_key(std::string(key));
    kv_client->Send(msg);
    msg = kv_client->Receive();
    //kv_client->Close();
    if (msg.has_value()) {
      strcpy (value, msg.value().c_str());
      return 0;
    } else {
      return 1;
    }
  } catch (int exception) {
    return -1;
  }
}

int kv739_put(char* key, char* value, char* old_value) {
  try {
    //if (kv_client->ConnectServer()) {
    //  return -1;
    //}
    KVMessage msg;
    msg.set_key(std::string(key));
    msg.set_value(std::string(value));
    kv_client->Send(msg);
    msg = kv_client->Receive();
    //kv_client->Close();
    if (msg.has_value()) {
      strcpy (old_value, msg.value().c_str());
      return 0;
    } else {
      return 1;
    }
  } catch (int exception) {
    return -1;
  }
}
