#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <string>
#include <any>
#include <unordered_map>
#include <set>

#include <glog/logging.h>

namespace warble {

// Class to handle Hook, Unhook and Event
// Connect event_type to event_function through a map
class EventHandler {
 public:
  // Default constructor
  EventHandler() : event_map_(), event_function_set_({kRegisteruser, kWarble,
                                                kFollow, kRead, kProfile}) {}
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
  // Return a pair: first - response payload
  //                second - true if success, false otherwise
  std::pair<std::any, bool> Event(int event_type, const std::any& payload);

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
};

}  // namespace warble

#endif  // EVENT_HANDLER_H