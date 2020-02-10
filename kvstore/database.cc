#include "database.h"

namespace kvstore {

bool DataBase::PutIntoStorage(const std::string &key, const std::string &value) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0) {
    return false;
  }
  storage_map_[key].push_back(value);
  return true;
}

std::optional<std::vector<std::string>>
DataBase::GetFromStorage(const std::string &key) {
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

}