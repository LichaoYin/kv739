#ifndef KV_DB_H
#define KV_DB_H

#include <sqlite3.h>
#include <string>

class KVDatabase {
 public:
  KVDatabase();
  bool Init();
  bool Put(std::string key, std::string value);
  bool Get(std::string key, std::string* value);
  bool Delete(std::string key);
 private:
  std::string db_name_;
  sqlite3* database_;
};

int CollectValue(void *data, int argc, char **argv, char **col_name);

#endif
