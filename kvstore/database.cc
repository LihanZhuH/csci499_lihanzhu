#include "kvstore/database.h"

#include <glog/logging.h>

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace kvstore {

bool DataBase::PutIntoStorage(const std::string &key,
                              const std::string &value) {
  std::scoped_lock(map_mutex_);
  // Error if the same key-value pair exists
  auto it =
      std::find(storage_map_[key].begin(), storage_map_[key].end(), value);
  if (it != storage_map_[key].end()) {
    LOG(WARNING) << "DataBase - PutIntoStorage: Duplicated entry.";
    return false;
  }
  storage_map_[key].push_back(value);
  return true;
}

std::optional<std::vector<std::string>> DataBase::GetFromStorage(
    const std::string &key) {
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
  LOG(WARNING) << "DataBase - RemoveFromStorage: No key found.";
  return false;
}

void DataBase::Serialize(std::ostream &os) const {
  std::scoped_lock(map_mutex_);

  // For each key in 'storage_map_', the format of serialized output is:
  // The first line is key and the second line is the number of values (n).
  // For the next n lines, each line is a value.
  for (auto kv : storage_map_) {
    os << kv.first << std::endl << kv.second.size() << std::endl;
    for (auto value : kv.second) {
      os << value << std::endl;
    }
  }
}

bool DataBase::Deserialize(std::istream &is) {
  std::string line, key;  // Line to be read and current key.
  int values_left = 0;    // Number of values left to be read.
  int state = 0;          // state == 0 means reading the key.
                          // state == 1 means reading the values size.
                          // state == 2 means reading the values.
  while (getline(is, line)) {
    // Uses a state machine to read different components.
    if (state == 0) {
      key = line;
      state = 1;
    } else if (state == 1) {
      try {
        values_left = std::stoi(line);
      } catch (std::invalid_argument &ia) {
        LOG(WARNING) << "DataBase - Deserialize: Failed to convert to int.";
        return false;  // Unable to convert to int
      }
      state = 2;
    } else {
      if (!PutIntoStorage(key, line)) {
        LOG(WARNING) << "DataBase - Deserialize: Failed to put key, value.";
        return false;  // Unable to put into storage
      }
      if (--values_left == 0) {
        state = 0;
      }
    }
  }
  return (state == 0);  // Succeeds only if all values of a key is read.
}

}  // namespace kvstore
