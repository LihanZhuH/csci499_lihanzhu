#include "event_handler.h"

namespace func {

bool EventHandler::Hook(int event_type, const std::string &event_function) {
  // Not a valid event function
  if (event_function_set_.find(event_function) == event_function_set_.end()) {
    LOG(WARNING) << "Event Handler: Invalid event function";
    return false;
  }
  std::scoped_lock(map_mutex_);
  // event_type has been used
  if (event_map_.find(event_type) != event_map_.end()) {
    LOG(WARNING) << "Event Handler: Event type has been used";
    return false;
  }
  event_map_.insert(std::pair<int, std::string>(event_type, event_function));
  return true;
}

bool EventHandler::Unhook(int event_type) {
  std::scoped_lock(map_mutex_);
  // event_type does not exist
  if (event_map_.find(event_type) == event_map_.end()) {
    LOG(WARNING) << "Event Handler: Event type does not exist";
    return false;
  }
  event_map_.erase(event_type);
  return true;
}

// TODO. HAVE NOT BEEN TESTED !!
bool EventHandler::Event(int event_type, const google::protobuf::Any& payload, 
                         google::protobuf::Any* reply) {
  std::scoped_lock(map_mutex_);
  // event_type does not exist
  google::protobuf::Any reply_payload;
  if (event_map_.find(event_type) == event_map_.end()) {
    return false;
  }
  std::string event_function = event_map_[event_type];

  // TODO: Call different functions depending on event type
  if (event_function == kRegisteruser) {
    LOG(INFO) << "Event Handler: registeruser called";
  } else if (event_function == kWarble) {
    LOG(INFO) << "Event Handler: warble called";
  } else if (event_function == kFollow) {
    LOG(INFO) << "Event Handler: follow called";
  } else if (event_function == kRead) {
    LOG(INFO) << "Event Handler: read called";
  } else if (event_function == kProfile) {
    LOG(INFO) << "Event Handler: profile called";
  } else {
    // Should never reach
    LOG(FATAL) << "Event Handler: no matching function";
    return false;
  }
  return true;
}

}  // namespace warble