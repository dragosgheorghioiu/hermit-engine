#ifndef PLUGIN_EVENT_FACTORY_H
#define PLUGIN_EVENT_FACTORY_H

#include "./SystemInfo.h"

#include <any>
#include <boost/dll/shared_library.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct SystemCallback {
  std::function<void(void *)> callback;
  std::string systemName;
  boost::dll::shared_library library;
  SystemCallback(std::function<void(void *)> callback, std::string systemName,
                 boost::dll::shared_library library)
      : callback(callback), systemName(systemName), library(library) {}
  ~SystemCallback() {
    callback = nullptr;
    systemName = "";
    library = boost::dll::shared_library();
  }
};

class EventFactoryInfo {
public:
  std::string name;
  void *(*createInstance)(...);
  void (*destroyInstance)(void *);
  std::vector<SystemCallback> callbacks;
  boost::dll::shared_library library;

  EventFactoryInfo(
      std::string name = "", void *(*createInstance)(...) = nullptr,
      void (*destroyInstance)(void *) = nullptr,
      boost::dll::shared_library library = boost::dll::shared_library())
      : name(name), createInstance(createInstance),
        destroyInstance(destroyInstance), library(library) {}
};

class PluginEventFactory {
private:
  std::unordered_map<std::string, EventFactoryInfo> events;

public:
  PluginEventFactory() = default;
  ~PluginEventFactory() = default;

  void loadEvents(const std::string &path);
  void loadEvent(const std::string &path);
  void unloadEvents();
  void unloadEvent(const std::string &name);
  template <typename... args_t>
  void triggerEvent(const std::string name, std::vector<std::any> args) {

    if (events.find(name) == events.end()) {
      return;
    }

    void *instance = events[name].createInstance(args);

    for (auto &callback : events[name].callbacks) {
      callback.callback(instance);
    }

    events[name].destroyInstance(instance);
  }
  void subscribe(const std::string &name, SystemInfo *systemInfo);

  void unsubscribe(const std::string &name, const std::string &systemName);
};

#endif
