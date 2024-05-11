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

  EventFactoryInfo info(getEventName(), createInstance, destroyInstance,
                        handle);
  events[info.name] = info;
  Logger::Log("Loaded plugin event: " + info.name);
}

void PluginEventFactory::unloadEvents() {
  for (auto &event : events) {
    event.second.callbacks.clear();
  }
  events.clear();
  Logger::Log("Unloaded events");
}

void PluginEventFactory::unloadEvent(const std::string &name) {
  auto it = events.find(name);
  if (it != events.end()) {
    it->second.library.unload();
    events.erase(it);
    Logger::Log("Unloaded event: " + name);
    return;
  }
  Logger::Warn("Event " + name + " not found!");
}

void PluginEventFactory::subscribe(const std::string &name,
                                   SystemInfo *systemInfo) {
  if (events.find(name) == events.end()) {
    Logger::Err("Could not find event type: " + name);
    return;
  }

  auto callback = systemInfo->instance->getCallback(name);
  if (!callback) {
    Logger::Err("Could not find callback for event: " + name);
    return;
  }

  events[name].callbacks.push_back(
      SystemCallback(callback, systemInfo->name, systemInfo->library));
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