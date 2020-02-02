#include "database.h"

bool DataBase::PutIntoStorage(const std::string &key, const std::string &value) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0) {
    return false;
  } else {
    storage_map_.insert({key, value});
    return true;
  }
}

const std::string DataBase::GetFromStorage(const std::string &key, bool *success) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0) {
    *success = true;
    return storage_map_[key];
  } else {
    *success = false;
    return "";
  }
}

bool DataBase::RemoveFromStorage(const std::string &key) {
  std::scoped_lock(map_mutex_);
  if (storage_map_.count(key) > 0) {
    storage_map_.erase(key);
    return true;
  } else {
    return false;
  }
}