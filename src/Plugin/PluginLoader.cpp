#include "PluginLoader.h"
#include "Logger/Logger.h"
#include "PluginComponentFactory.h"
#include "PluginSystemInterface.h"

#include <memory>

// constructor
PluginLoader::~PluginLoader() {
  unloadComponents();
  unloadSystems();
}

// function that loads all the given shared libraries at the given path as
// plugins
void PluginLoader::loadSystems(const std::string &path) {
  // Load all plugins from the given path
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      loadSystem(entry.path().string());
    }
  }
}

// function that returns the plugin with the given name
void PluginLoader::loadSystem(const std::string &path) {
  boost::dll::shared_library handle(path);
  if (!handle) {
    Logger::Err("Failed to load plugin: " + path);
    return;
  }

  auto createInstance = handle.get<void *()>("createInstance");
  if (!createInstance) {
    Logger::Err("Failed to load plugin: " + path);
    return;
  }

  PluginSystemInterface *instance = (PluginSystemInterface *)createInstance();
  if (!instance) {
    Logger::Err("Failed to load plugin: " + path);
    return;
  }

  std::shared_ptr<PluginSystemInterface> plugin(instance);

  PluginInfo info;
  info.library = handle;
  info.system = plugin;
  info.name = plugin->GetName();
  plugins[info.name] = info;
  Logger::Log("Loaded plugin: " + info.name);
}

// function that loads all the components from the given path
void PluginLoader::loadComponents(const std::string &path) {
  componentFactory.loadComponents(path);
}

// function that loads the component with the given path
void PluginLoader::loadComponent(const std::string &path) {
  componentFactory.loadComponent(path);
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
void PluginLoader::callSystemUpdate(const std::string &name,
                                    std::vector<void *> params) {
  auto it = plugins.find(name);
  if (it != plugins.end()) {
    it->second.system->Update(params);
  }
}

// function that returns the component factory
PluginComponentFactory PluginLoader::getComponentFactory() {
  return componentFactory;
}

void PluginLoader::DestroySelf() { delete this; }
