#include "Plugin/PluginLoader.h"

void PluginLoader::loadPlugins(const std::string &path) {
  // Load all plugins from the given path
  boost::dll::shared_library handle(path);
  if (!handle) {
    Logger::Err("Failed to load plugin: " + path);
    return;
  }
  Logger::Warn("Loaded plugin: " + path);

  auto printHello = handle.get<void()>("printHello");
  if (!printHello) {
    Logger::Err("Failed to load plugin: " + path);
    return;
  }
  printHello();

  //   PluginInfo info;
  //   info.name = path;
  //   info.library = handle;
  //   // info.system = plugin;
  //
  //   plugins.push_back(info);
  //   Logger::Log("Loaded plugin: " + path);
}
