#include "PluginComponentFactory.h"
#include "../Logger/Logger.h"
#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <string>

void PluginComponentFactory::loadComponents(const std::string &path,
                                            sol::state &type) {
  int i = size;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".so") {
      loadComponent(entry.path().string(), i++, type);
    }
  }
  this->size = i;
}

void PluginComponentFactory::loadComponent(const std::string &path, int id,
                                           sol::state &lua) {
  boost::dll::shared_library handle;
  try {
    handle = boost::dll::shared_library(path);
  } catch (const std::exception &e) {
    Logger::Err("Failed to load component: " + path);
    return;
  }

  void *(*createInstance)(...) = nullptr;
  try {
    createInstance = handle.get<void *(...)>("createInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load component createInstance: " + path);
    return;
  }

  void (*destroyInstance)(void *) = nullptr;
  try {
    destroyInstance = handle.get<void(void *)>("destroyInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load component destroyInstance: " + path);
    return;
  }

  const char *(*getComponentName)() = nullptr;
  try {
    getComponentName = handle.get<const char *()>("getComponentName");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load component getComponentName: " + path);
    return;
  }

  void (*addLuaBindings)(sol::state &) = nullptr;
  try {
    addLuaBindings = handle.get<void(sol::state &)>("addLuaBindings");
    addLuaBindings(lua);
  } catch (const std::exception &e) {
    Logger::Warn("Failed to load component addLuaBindings: " + path);
  }

  ComponentFactoryInfo info(id, path, getComponentName(), createInstance,
                            destroyInstance, handle);
  components[info.getName()] = info;

  Logger::Log("Loaded component: " + info.getName());
}

void PluginComponentFactory::unloadComponents() {
  for (auto &component : components) {
    component.second.getLibrary().unload();
  }
  components.clear();
  Logger::Log("Unloaded components");
}

void PluginComponentFactory::unloadComponent(const std::string &name) {
  auto it = components.find(name);
  if (it != components.end()) {
    Logger::Log("Unloaded component: " + name);
    components.erase(it);
  }
  Logger::Warn("Component " + name + " not found!");
}

ComponentFactoryInfo &
PluginComponentFactory::getComponentFactoryInfo(const std::string &name) {
  if (components.find(name) == components.end()) {
    Logger::Err("Component not found: " + name);
    exit(1);
  }
  return components[name];
}

void ComponentInfo::createLuaUserType(sol::state &lua) {
  lua.new_usertype<ComponentInfo>("component_info", "id", &ComponentInfo::id,
                                  "name", &ComponentInfo::name, "instance",
                                  &ComponentInfo::instance);
}
