#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "Plugin/PluginInfo.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>
#include <vector>

class PluginLoader {
private:
  std::filesystem::path pluginPath;
  std::vector<PluginInfo> plugins;

public:
  PluginLoader() : pluginPath(){};

  void loadPlugins(const std::string &path);

  void loadPlugin(const std::string &path);

  void unloadPlugins() {
    // Unload all plugins
    for (auto &plugin : plugins) {
      plugin.library.unload();
    }
    plugins.clear();
  }

  void callPluginUpdate() {
    Logger::Log("Calling plugin update");
    for (auto &plugin : plugins) {
      plugin.system->Update();
    }
  }
};

#endif
