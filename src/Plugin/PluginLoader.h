#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "Plugin/PluginInfo.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <vector>

// class IPluginLoader {
// public:
//   virtual void loadPlugins(const std::string &path) = 0;
//   virtual void unloadPlugins() = 0;
// };

class PluginLoader {
private:
  std::vector<PluginInfo> plugins;

public:
  PluginLoader() = default;

  void loadPlugins(const std::string &path);

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
