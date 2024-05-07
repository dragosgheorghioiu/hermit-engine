#ifndef PLUGIN_EVENT_FACTORY_H
#define PLUGIN_EVENT_FACTORY_H

#include "../Logger/Logger.h"

#include <string>
#include <unordered_map>
#include <vector>

struct SystemCallback {
  void (*callback)(void *);
  std::string systemName;
};

struct EventFactoryInfo {
  std::string name;
  void *(*createInstance)(...);
  void (*destroyInstance)(void *);
  std::vector<SystemCallback> callbacks;

  EventFactoryInfo(std::string name = "",
                   void *(*createInstance)(...) = nullptr,
                   void (*destroyInstance)(void *) = nullptr)
      : name(name), createInstance(createInstance),
        destroyInstance(destroyInstance) {}

  std::string getName() { return name; }
  void *(*getCreateInstance())(...) { return createInstance; }
  void (*getDestroyInstance())(void *) { return destroyInstance; }
};

class PluginEventFactory {
private:
  std::unordered_map<std::string, EventFactoryInfo> events;
  int size;

public:
  PluginEventFactory() { Logger::Log("EventFactory constructor"); }
  ~PluginEventFactory() { Logger::Log("EventFactory destructor"); }

  void loadEvents(const std::string &path);
  void loadEvent(const std::string &path);
  void unloadEvents();
  void unloadEvent(const std::string &name);
  template <typename... Args>
  void callEvent(const std::string &name, Args &&...args) {
    if (events.find(name) == events.end()) {
      return;
    }

    auto createInstance = events[name].getCreateInstance();
    auto destroyInstance = events[name].getDestroyInstance();

    void *instance = createInstance(args...);

    for (auto &callback : events[name].callbacks) {
      callback.callback(instance);
    }

    destroyInstance(instance);
  }
  void subscribe(const std::string &name, void (*callback)(void *),
                 const std::string &systemName);

  void unsubscribe(const std::string &name, const std::string &systemName);
};

#endif
