#include "func/event_handler.h"

namespace func {

bool EventHandler::Hook(int event_type, const std::string& event_function) {
  // Not a valid event function
  if (event_function_set_.find(event_function) == event_function_set_.end()) {
    LOG(WARNING) << "Event Handler - Hook: Invalid event function";
    return false;
  }
  std::scoped_lock(map_mutex_);
  // event_type has been used
  if (event_map_.find(event_type) != event_map_.end()) {
    LOG(WARNING) << "Event Handler - Hook: Event type has been used";
    return false;
  }
  event_map_.insert(std::pair<int, std::string>(event_type, event_function));
  LOG(INFO) << "Event Handler - Hook: Event " << event_function << " hooked to "
            << event_type;
  return true;
}

bool EventHandler::Unhook(int event_type) {
  std::scoped_lock(map_mutex_);
  // event_type does not exist
  if (event_map_.find(event_type) == event_map_.end()) {
    LOG(WARNING) << "Event Handler - Unhook: Event type does not exist";
    return false;
  }
  event_map_.erase(event_type);
  LOG(INFO) << "Event Handler - Unhook: Event " << event_type << " unhooked";
  return true;
}

std::optional<google::protobuf::Any> EventHandler::Event(
    int event_type, const google::protobuf::Any& payload) {
  std::string event_function;
  {
    std::scoped_lock(map_mutex_);
    // event_type does not exist
    if (event_map_.find(event_type) == event_map_.end()) {
      LOG(WARNING) << "Event Handler - Event: Unknown event";
      return std::nullopt;
    }
    event_function = event_map_[event_type];
  }
  google::protobuf::Any reply_payload;
  // Call different Warble functions depending on event type
  if (event_function == kRegisteruser) {
    LOG(INFO) << "Event Handler - Event: registeruser called";
    warble::RegisteruserRequest request;
    warble::RegisteruserReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (Registeruser(kvstore_client_, request, &reply)) {
      reply_payload.PackFrom(reply);
      LOG(INFO) << "Event Handler - Event: Successful registeruser call";
      return reply_payload;
    }
  } else if (event_function == kWarble) {
    LOG(INFO) << "Event Handler - Event: warble called";
    warble::WarbleRequest request;
    warble::WarbleReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (NewWarble(kvstore_client_, request, &reply)) {
      reply_payload.PackFrom(reply);
      LOG(INFO) << "Event Handler - Event: Successful warble call";
      return reply_payload;
    }
  } else if (event_function == kFollow) {
    LOG(INFO) << "Event Handler - Event: follow called";
    warble::FollowRequest request;
    warble::FollowReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (Follow(kvstore_client_, request, &reply)) {
      reply_payload.PackFrom(reply);
      LOG(INFO) << "Event Handler - Event: Successful follow call";
      return reply_payload;
    }
  } else if (event_function == kRead) {
    LOG(INFO) << "Event Handler - Event: read called";
    warble::ReadRequest request;
    warble::ReadReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (Read(kvstore_client_, request, &reply)) {
      reply_payload.PackFrom(reply);
      LOG(INFO) << "Event Handler - Event: Successful read call";
      return reply_payload;
    }
  } else if (event_function == kProfile) {
    LOG(INFO) << "Event Handler - Event: profile called";
    warble::ProfileRequest request;
    warble::ProfileReply reply;
    payload.UnpackTo(&request);  // Unpack Any

    if (Profile(kvstore_client_, request, &reply)) {
      reply_payload.PackFrom(reply);
      LOG(INFO) << "Event Handler - Event: Successful profile call";
      return reply_payload;
    }
  } else {
    // Should never reach
    LOG(FATAL) << "Event Handler - Event: Unknown event not catched";
  }
  return std::nullopt;
}

}  // namespace func
