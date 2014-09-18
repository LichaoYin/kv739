#ifndef KV739_H
#define KV739_H

int kv739_init(char* server);
int kv739_get(char* key, char* value);
int kv739_put(char* key, char* value, char* old_value);

#endif
