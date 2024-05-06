#include "PluginComponentFactory.h"
#include "../Logger/Logger.h"
#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <string>

void PluginComponentFactory::loadComponents(const std::string &path) {
  int i = size;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".so") {
      loadComponent(entry.path().string(), i++);
    }
  }
  this->size = i;
}

void PluginComponentFactory::loadComponent(const std::string &path, int id) {
  boost::dll::shared_library handle(path);
  if (!handle) {
    Logger::Err("Failed to load component: " + path);
    return;
  }

  auto createInstance = handle.get<void *(...)>("createInstance");
  if (!createInstance) {
    Logger::Err("Failed to load component: " + path);
    return;
  }

  auto destroyInstance = handle.get<void(void *)>("destroyInstance");
  if (!destroyInstance) {
    Logger::Err("Failed to load component: " + path);
    return;
  }

  auto getComponentName = handle.get<const char *()>("getComponentName");
  if (!getComponentName) {
    Logger::Err("Failed to load component: " + path);
    return;
  }

  ComponentFactoryInfo info(id, path, getComponentName(), createInstance,
                            destroyInstance, handle);
  components[info.getName()] = info;

  Logger::Log("Loaded component: " + info.getName());
}

void PluginComponentFactory::unloadComponents() {
  Logger::Log("Unloading components");
  components.clear();
}

void PluginComponentFactory::unloadComponent(const std::string &name) {
  auto it = components.find(name);
  if (it != components.end()) {
    Logger::Log("Unloading component: " + name);
    components.erase(it);
  }
}

ComponentFactoryInfo &
PluginComponentFactory::getComponentFactoryInfo(const std::string &name) {
  if (components.find(name) == components.end()) {
    Logger::Err("Component not found: " + name);
    exit(1);
  }
  return components[name];
}
