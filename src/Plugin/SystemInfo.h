#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "Logger/Logger.h"
#include "Plugin/SystemInstance.h"
#include <boost/dll.hpp>
#include <string>

struct SystemInfo {
  std::string name;
  SystemInstance *instance;
  void (*destroyInstance)(void *);
  void (*update)();
  boost::dll::shared_library library;

  SystemInfo(std::string name = "", SystemInstance *instance = nullptr,
             boost::dll::shared_library library = boost::dll::shared_library(),
             void (*destroyInstance)(void *) = nullptr,
             void (*update)() = nullptr)
      : name(name), instance(instance), library(library),
        destroyInstance(destroyInstance), update(update) {}
  ~SystemInfo() {
    if (!instance) {
      Logger::Err("System instance is null");
      return;
    }
    if (!destroyInstance) {
      Logger::Err("Destroy instance function is null");
      return;
    }
    destroyInstance(instance);
  }
};

#endif
