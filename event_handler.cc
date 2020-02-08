#include "event_handler.h"

using std::string;
using std::any;
using std::pair;

bool EventHandler::Hook(int event_type, const string &event_function) {
  // Not a valid event function
  if (event_function_set_.find(event_function) == event_function_set_.end()) {
    LOG(WARNING) << "Invalid event function";
    return false;
  }
  // event_type has been used
  if (event_map_.find(event_type) != event_map_.end()) {
    LOG(WARNING) << "Event type has been used";
    return false;
  }
  event_map_.insert(pair<int, string>(event_type, event_function));
  return true;
}

bool EventHandler::Unhook(int event_type) {
  // event_type does not exist
  if (event_map_.find(event_type) == event_map_.end()) {
    LOG(WARNING) << "Event type does not exist";
    return false;
  }
  event_map_.erase(event_type);
  return true;
}

// UNDER CONSTRUCTION. HAVE NOT BEEN TESTED !!
pair<any, bool> EventHandler::Event(int event_type, const any& payload) {
  // event_type does not exist
  if (event_map_.find(event_type) == event_map_.end()) {
    return pair<any, bool>(0, false);
  }
  string event_function = event_map_[event_type];

  // TODO: Call different functions depending on event type
  if (event_function == kRegisteruser) {
    LOG(INFO) << "registeruser called";
  } else if (event_function == kWarble) {
    LOG(INFO) << "warble called";
  } else if (event_function == kFollow) {
    LOG(INFO) << "follow called";
  } else if (event_function == kRead) {
    LOG(INFO) << "read called";
  } else if (event_function == kProfile) {
    LOG(INFO) << "profile called";
  } else {
    // Should never reach
    LOG(FATAL) << "no matching function";
    return pair<any, bool>(0, false);
  }
  return pair<any, bool>(0, true);
}