#ifndef KVSTORE_DATABASE_H_
#define KVSTORE_DATABASE_H_

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace kvstore {

// Database storage for thread-safe put, get, and remove
// Supports multiple entries with the same key
class DataBase {
 public:
  // Initialize map and mutex
  DataBase() : storage_map_(), map_mutex_() {}
  ~DataBase() {}

  // Disable move and copy
  DataBase(const DataBase &) = delete;
  DataBase &operator=(const DataBase &) = delete;

  // Put 'key' and 'value' into storage
  // Return true if correctly stored, false otherwise
  bool PutIntoStorage(const std::string &key, const std::string &value);

  // Return an optional with all values corresponding to 'key'
  // Optional is null if no key found
  std::optional<std::vector<std::string>> GetFromStorage(
      const std::string &key);

  // Remove key in storage
  // Return true if key exists, false otherwise
  bool RemoveFromStorage(const std::string &key);

  // Serializes 'storage_map_' into string and puts it in 'os'
  void Serialize(std::ostream &os) const;

  // Deserializes from 'is' and replaces 'storage_map_'
  bool Deserialize(std::istream &is);

 private:
  // Local storage of key-value pairs
  std::unordered_map<std::string, std::vector<std::string>> storage_map_;

  // Mutex for thread-safe
  std::mutex map_mutex_;
};

}  // namespace kvstore

#endif  // KVSTORE_DATABASE_H_
