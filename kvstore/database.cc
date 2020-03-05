#include "kvstore/database.h"

#include <algorithm>

namespace kvstore {

bool DataBase::PutIntoStorage(const std::string &key,
    const std::string &value) {
  std::scoped_lock(map_mutex_);
  // Error if the same key-value pair exists
  auto it = std::find(storage_map_[key].begin(),
                      storage_map_[key].end(),
                      value);
  if (it != storage_map_[key].end()) {
    return false;
  }
  storage_map_[key].push_back(value);
  return true;
}

std::optional<std::vector<std::string>> DataBase::GetFromStorage
    (const std::string &key) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0 && !storage_map_[key].empty()) {
    return storage_map_[key];
  }
  return std::nullopt;
}

bool DataBase::RemoveFromStorage(const std::string &key) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0) {
    storage_map_.erase(key);
    return true;
  }
  return false;
}

}  // namespace kvstore
