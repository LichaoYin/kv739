#include <iostream>
#include <stdio.h>
#include <string.h>

#include "kv739.h"

int main() {
  char * server = "128.105.14.207:5001";
  int init = kv739_init(server);
  char buffer[4096];
  int rc = -100;
  rc = kv739_get("key1",(char*) buffer);
  if (rc == 0) {
    printf("[key1, %s]\n", buffer);
  } else if (rc == 1) {
    printf("No value is found\n");
  } else {
    printf("Error\n");
  }
  memset(buffer, 0, sizeof(buffer));
  rc = kv739_put("key2", "value2", (char*) buffer);
  printf("rc = %d\n", rc);

  memset(buffer, 0, sizeof(buffer));
  rc = kv739_put("key2", "value3", (char*) buffer);
  printf("rc = %d\n", rc);
  if (rc == 0) {
    printf("Old [key1, %s]\n", buffer);
  }


  exit(1);
}
