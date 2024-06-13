#include "PluginLoader.h"
#include "PluginComponentFactory.h"
#include "SystemInfo.h"
#include "SystemInstance.h"
#include <iostream>

// constructor
PluginLoader::~PluginLoader() = default;

// function that loads all the given shared libraries at the given path as
// plugins
void PluginLoader::loadSystems(const std::string &path, RegistryType *registry,
                               sol::state *lua) {
  // Load all plugins from the given path
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      loadSystem(entry.path().string(), registry, lua);
    }
  }
}

// function that returns the plugin with the given name
void PluginLoader::loadSystem(const std::string &path, RegistryType *registry,
                              sol::state *lua) {
  boost::dll::shared_library handle;

  // Load the plugin
  try {
    handle = boost::dll::shared_library(path);
  } catch (const std::exception &e) {
    Logger::Err("Failed to load plugin: " + path);
    std::cerr << e.what() << std::endl;
    return;
  }

  // Get the createInstance function
  void *(*createInstance)();
  try {
    createInstance = handle.get<void *()>("createInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load plugin system createInstance: " + path);
    return;
  }

  // Get the destroyInstance function
  void (*destroyInstance)(void *);
  try {
    destroyInstance = handle.get<void(void *)>("destroyInstance");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load plugin system destroyInstance: " + path);
    return;
  }

  // Get the getName function
  const char *(*getName)();
  try {
    getName = handle.get<const char *()>("getName");
  } catch (const std::exception &e) {
    Logger::Err("Failed to load plugin system getName: " + path);
    return;
  }

  const char **(*requiredComponents)();
  try {
    requiredComponents = handle.get<const char **()>("getRequiredComponents");
  } catch (const std::exception &e) {
    Logger::Warn("Failed to load plugin system getRequiredComponents: " + path);
  }

  const char **requiredComponentsArray = requiredComponents();

  SystemInstance *instance = static_cast<SystemInstance *>(createInstance());

  registry->addPluginSystem(createInstance, getName(), destroyInstance, handle,
                            requiredComponentsArray, lua);
}

// function that loads all the components from the given path
void PluginLoader::loadComponents(const std::string &path, sol::state &lua) {
  componentFactoryList.loadComponents(path, lua);
}

// function that loads all the events from the given path
void PluginLoader::loadEvents(const std::string &path) {
  eventFactory.loadEvents(path);
}

// function that loads an event from the given path
void PluginLoader::loadEvent(const std::string &path) {
  eventFactory.loadEvent(path);
}

// function that loads the component with the given path
void PluginLoader::loadComponent(const std::string &path, sol::state &lua) {
  componentFactoryList.loadComponent(path, componentFactoryList.getSize(), lua);
}

// function that unloads all the plugins
void PluginLoader::unloadSystems() {
  Logger::Log("Unloaded systems");
  for (auto &plugin : systemsMap) {
    plugin.second.library.unload();
  }
  systemsMap.clear();
}

// function that unloads all the components
void PluginLoader::unloadComponents() {
  componentFactoryList.unloadComponents();
}

// function that unloads all the events
void PluginLoader::unloadEvents() { eventFactory.unloadEvents(); }

// function that returns the plugin with the given name
void PluginLoader::unloadSystem(const std::string &name) {
  auto it = systemsMap.find(name);
  if (it != systemsMap.end()) {
    Logger::Log("Unloaded system: " + name);
    systemsMap.erase(it);
    it->second.library.unload();
  }
}

// function that calls the Update function of the plugin with the given name
void PluginLoader::callSystemUpdate(RegistryType *registry,
                                    const std::string &name,
                                    std::vector<void *> params) {
  auto it = registry->getPluginSystem(name);
  it.instance->update(params);
}

// function that returns the component factory
ComponentFactoryList &PluginLoader::getComponentFactory() {
  return componentFactoryList;
}

// function that returns the event factory
PluginEventFactory &PluginLoader::getEventFactory() { return eventFactory; }

// function that returns the component info with the given name
ComponentFactoryInfo &PluginLoader::getComponentInfo(const std::string &name) {
  return componentFactoryList.getComponentFactoryInfo(name);
}

void PluginLoader::DestroySelf() { delete this; }
