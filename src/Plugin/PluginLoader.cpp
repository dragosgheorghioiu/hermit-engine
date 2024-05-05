#include "PluginLoader.h"
#include "Logger/Logger.h"
#include "Plugin/SystemInfo.h"
#include "Plugin/SystemInstance.h"
#include "PluginComponentFactory.h"
#include <iostream>

// constructor
PluginLoader::~PluginLoader() {
  unloadComponents();
  unloadSystems();
}

// function that loads all the given shared libraries at the given path as
// plugins
void PluginLoader::loadSystems(const std::string &path,
                               RegistryType *registry) {
  // Load all plugins from the given path
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      loadSystem(entry.path().string(), registry);
    }
  }
}

// function that returns the plugin with the given name
void PluginLoader::loadSystem(const std::string &path, RegistryType *registry) {
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
    Logger::Err("Failed to load plugin system getRequiredComponents: " + path);
    return;
  }

  const char **requiredComponentsArray = requiredComponents();

  registry->addPluginSystem(createInstance, getName(), destroyInstance, handle,
                            requiredComponentsArray);
}

// function that loads all the components from the given path
void PluginLoader::loadComponents(const std::string &path) {
  componentFactory.loadComponents(path);
}

// function that loads the component with the given path
void PluginLoader::loadComponent(const std::string &path) {
  componentFactory.loadComponent(path, componentFactory.getSize());
}

// function that unloads all the plugins
void PluginLoader::unloadSystems() {
  Logger::Log("Unloading systems");
  plugins.clear();
}

// function that unloads all the components
void PluginLoader::unloadComponents() { componentFactory.unloadComponents(); }

// function that returns the plugin with the given name
void PluginLoader::unloadSystem(const std::string &name) {
  auto it = plugins.find(name);
  if (it != plugins.end()) {
    Logger::Log("Unloading system: " + name);
    plugins.erase(it);
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
PluginComponentFactory &PluginLoader::getComponentFactory() {
  return componentFactory;
}

void PluginLoader::DestroySelf() { delete this; }
