#include "PluginEventFactory.h"
#include "../Logger/Logger.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>

void PluginEventFactory::loadEvents(const std::string &path) {
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    loadEvent(entry.path().string());
  }
}

void PluginEventFactory::loadEvent(const std::string &path) {
  boost::dll::shared_library handle;
  try {
    handle = boost::dll::shared_library(path);
  } catch (const std::exception &e) {
    Logger::Err("Failed to load event: " + path);
    return;
  }

  void *(*createInstance)(...) = nullptr;
  try {
    createInstance = handle.get<void *(...)>("createInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load event createInstance: " + path);
    return;
  }

  void (*destroyInstance)(void *) = nullptr;
  try {
    destroyInstance = handle.get<void(void *)>("destroyInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load event destroyInstance: " + path);
    return;
  }

  const char *(*getEventName)() = nullptr;
  try {
    getEventName = handle.get<const char *()>("getEventName");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load event getEventName: " + path);
    return;
  }

  EventFactoryInfo info(getEventName(), createInstance, destroyInstance);
  events[info.getName()] = info;
}

void PluginEventFactory::unloadEvents() {
  for (auto &event : events) {
    unloadEvent(event.first);
  }
}

void PluginEventFactory::unloadEvent(const std::string &name) {
  events.erase(name);
}

void PluginEventFactory::subscribe(const std::string &name,
                                   void (*callback)(void *),
                                   const std::string &systemName) {
  if (events.find(name) == events.end()) {
    Logger::Log("Could not find event type: " + name);
    return;
  }

  events[name].callbacks.push_back({callback, systemName});
}

void PluginEventFactory::unsubscribe(const std::string &name,
                                     const std::string &systemName) {
  if (events.find(name) == events.end()) {
    Logger::Log("Could not find event type: " + name);
    return;
  }

  auto &callbacks = events[name].callbacks;
  callbacks.erase(
      std::remove_if(callbacks.begin(), callbacks.end(),
                     [&](const SystemCallback systemCallback) -> bool {
                       return systemCallback.systemName == systemName;
                     }),
      callbacks.end());
}
