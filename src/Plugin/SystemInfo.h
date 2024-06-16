#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "../Logger/Logger.h"
#include "SystemInstance.h"
#include <boost/dll.hpp>
#include <string>

struct SystemInfo {
  std::string name;
  SystemInstance *instance;
  void (*destroyInstance)(void *);
  boost::dll::shared_library library;
  const char **requiredComponents;

  SystemInfo(std::string name = "", SystemInstance *instance = nullptr,
             boost::dll::shared_library library = boost::dll::shared_library(),
             void (*destroyInstance)(void *) = nullptr,
             const char **requiredComponents = nullptr)
      : name(name), instance(instance), library(library),
        destroyInstance(destroyInstance),
        requiredComponents(requiredComponents) {}
  ~SystemInfo() {
    if (!instance) {
      Logger::Err("System instance is null");
      return;
    }
    if (!destroyInstance) {
      Logger::Err("Destroy instance function is null");
      return;
    }
    instance->removeAllEntitiesFromSystem();
    destroyInstance(instance);
    library.unload();
  }
};

#endif
