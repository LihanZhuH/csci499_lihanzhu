#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <string>
#include <any>
#include <unordered_map>
#include <set>
#include <mutex>
#include <memory>
#include <optional>

#include <glog/logging.h>
#include <google/protobuf/any.pb.h>

#include "warble/warble_func.h"
#include "warble/warble.pb.h"
#include "kvstore/kvstore_client.h"

namespace func {

// Class to handle Hook, Unhook and Event
// Connect event_type to event_function through a map
class EventHandler {
 public:
  // Default constructor
  EventHandler(std::shared_ptr<kvstore::KVStoreClientAbstract> client)
     : event_map_(),
       event_function_set_({kRegisteruser, kWarble, kFollow, kRead, kProfile}),
       map_mutex_(),
       warble_func_(client) {}
  ~EventHandler() {}
  
  // Disable move and copy
  EventHandler(const EventHandler&) = delete;
  EventHandler& operator=(const EventHandler&) = delete;

  // Hook an event type with event function.
  // Return true if success, false otherwise
  bool Hook(int event_type, const std::string &event_function);

  // Unhook and event type
  // Return true if success, false otherwise
  bool Unhook(int event_type);

  // Call corresponding function to event type with payload
  // Return true if success, false otherwise
  std::optional<google::protobuf::Any> Event(int event_type,
      const google::protobuf::Any& payload);

  // Function names
  const std::string kRegisteruser = "registeruser";
  const std::string kWarble = "warble";
  const std::string kFollow = "follow";
  const std::string kRead = "read";
  const std::string kProfile = "profile";

 private:
  // Map with key: event type, value: event function name
  std::unordered_map<int, std::string> event_map_;

  // Set of valid function names
  std::set<std::string> event_function_set_;

  // Mutex for thread-safe
  std::mutex map_mutex_;

  // Warble functionalities
  warble::WarbleFunc warble_func_;
};

}  // namespace func

#endif  // EVENT_HANDLER_H