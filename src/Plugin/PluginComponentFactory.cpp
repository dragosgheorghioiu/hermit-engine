#include "PluginComponentFactory.h"
#include "../Logger/Logger.h"
#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <string>

void PluginComponentFactory::loadComponents(const std::string &path) {
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".so") {
      loadComponent(entry.path().string());
    }
  }
}

void PluginComponentFactory::loadComponent(const std::string &path) {
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

  ComponentInfo info;
  info.name = std::string(getComponentName());
  info.path = path;
  info.createInstance = (void *(*)(...))createInstance;
  info.destroyInstance = (void (*)(void *))destroyInstance;
  components[info.path] = info;

  void *instance = info.createInstance(42, "nu ma mai doare nimic");
  info.destroyInstance(instance);

  Logger::Warn("Loaded component: " + info.name);
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

ComponentInfo
PluginComponentFactory::getComponentInfo(const std::string &name) {
  return components[name];
}
