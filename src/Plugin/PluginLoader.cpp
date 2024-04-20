#include "Plugin/PluginLoader.h"
#include "Plugin/PluginSystemInterface.h"
#include <memory>

// function that loads all the given shared libraries at the given path as
// plugins
void PluginLoader::loadPlugins(const std::string &path) {
  // Load all plugins from the given path
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      loadPlugin(entry.path().string());
    }
  }
}

// function that returns the plugin with the given name
void PluginLoader::loadPlugin(const std::string &path) {
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
  info.name = plugin->GetName();
  info.library = handle;
  info.system = plugin;
  plugins[info.name] = info;
  Logger::Log("Loaded plugin: " + info.name);
}

// function that unloads all the plugins
void PluginLoader::unloadPlugins() {
  Logger::Log("Unloading plugins");
  plugins.clear();
}

// function that returns the plugin with the given name
void PluginLoader::unloadPlugin(const std::string &name) {
  auto it = plugins.find(name);
  if (it != plugins.end()) {
    Logger::Log("Unloading plugin: " + name);
    plugins.erase(it);
    it->second.library.unload();
  }
}

// function that calls the Update function of the plugin with the given name
void PluginLoader::callPluginUpdate(const std::string &name,
                                    std::vector<void *> params) {
  auto it = plugins.find(name);
  if (it != plugins.end()) {
    it->second.system->Update(params);
  }
}
