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
  LOG(INFO) << "Event Handler: Event " << event_function << " hooked";
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
  LOG(INFO) << "Event Handler: Event " << event_type << " unhooked";
  return true;
}

std::optional<google::protobuf::Any>
EventHandler::Event(int event_type, const google::protobuf::Any& payload) {
  std::string event_function;
  {
    std::scoped_lock(map_mutex_);
    // event_type does not exist
    if (event_map_.find(event_type) == event_map_.end()) {
      return std::nullopt;
    }
    event_function = event_map_[event_type];
  }
  google::protobuf::Any reply_payload;
  // Call different Warble functions depending on event type
  if (event_function == kRegisteruser) {
    LOG(INFO) << "Event Handler: registeruser called";
    warble::RegisteruserRequest request;
    warble::RegisteruserReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (warble_func_.Registeruser(request, &reply)) {
      reply_payload.PackFrom(reply);
      return reply_payload;
    }
  } else if (event_function == kWarble) {
    LOG(INFO) << "Event Handler: warble called";
    warble::WarbleRequest request;
    warble::WarbleReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (warble_func_.NewWarble(request, &reply)) {
      reply_payload.PackFrom(reply);
      return reply_payload;
    }
  } else if (event_function == kFollow) {
    LOG(INFO) << "Event Handler: follow called";
    warble::FollowRequest request;
    warble::FollowReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (warble_func_.Follow(request, &reply)) {
      reply_payload.PackFrom(reply);
      return reply_payload;
    }
  } else if (event_function == kRead) {
    LOG(INFO) << "Event Handler: read called";
    warble::ReadRequest request;
    warble::ReadReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (warble_func_.Read(request, &reply)) {
      reply_payload.PackFrom(reply);
      return reply_payload;
    }
  } else if (event_function == kProfile) {
    LOG(INFO) << "Event Handler: profile called";
    warble::ProfileRequest request;
    warble::ProfileReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (warble_func_.Profile(request, &reply)) {
      reply_payload.PackFrom(reply);
      return reply_payload;
    }
  } else {
    // Should never reach
    LOG(FATAL) << "Event Handler: no matching function";
  }
  return std::nullopt;
}

}  // namespace warble