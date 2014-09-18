#include "kv_db.h"

#include <stdio.h>
#include <vector>

KVDatabase::KVDatabase() {
}

bool KVDatabase::Init() {
  int rc = sqlite3_open("db/kv739_data.db", &database_);
  if (rc != 0) {
    perror("[Error] Failed on opening database file");
    sqlite3_close(database_);
    return false;
  }
  return true;
}

bool KVDatabase::Get(std::string key, std::string* value) {
  std::string sql = "select value from data where key = '" + key + "';";
  char * error_msg = 0;
  int rc = sqlite3_exec(database_, sql.c_str(), CollectValue, value, &error_msg );
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    return false;
  }
  return true;
}

bool KVDatabase::Put(std::string key, std::string value) {
  std::string sql = "insert into data values (" + key + "," + value + ");";
  char * error_msg = 0;
  int rc = sqlite3_exec(database_, sql.c_str(), NULL, NULL, &error_msg );
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    return false;
  }
  return true;
}

bool KVDatabase::Delete(std::string key) {
  std::string sql = "delete from data where key = '" + key + "';";
  printf("%s\n", sql.c_str());
  char * error_msg = 0;
  int rc = sqlite3_exec(database_, sql.c_str(), NULL, NULL, &error_msg );
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    return false;
  }
  return true;
}

int CollectValue(void *data, int argc, char **argv, char **col_name) {
  if (argc != 1)
    return -1;
  *(std::string *)data = std::string(argv[0]);
  return 0;
}
