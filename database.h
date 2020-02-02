#ifndef DATABASE_H_
#define DATABASE_H_

#include <unordered_map>
#include <string>
#include <mutex>

// Data Storage
class DataBase {
 public:
  // Initialize map and mutex
  DataBase(): storage_map_(), map_mutex_() {}
  ~DataBase() {}

  // Put 'key' and 'value' into storage
  // Return true if correctly stored, false otherwise
  bool PutIntoStorage(const std::string &key, const std::string &value);

  // Return the value corresponding to 'key'
  // 'success' is set to true if found, false otherwise
  const std::string GetFromStorage(const std::string &key, bool *success);

  // Remove key in storage
  // Return true if key exists, false otherwise
  bool RemoveFromStorage(const std::string &key);

 private:
  // Local storage of key-value pairs
  std::unordered_map<std::string, std::string> storage_map_;

  // Mutex for thread-safe
  std::mutex map_mutex_;
};

#endif  // DATABASE_H_