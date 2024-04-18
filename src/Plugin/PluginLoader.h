#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "Plugin/PluginInfo.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>
#include <unordered_map>

class PluginLoader {
private:
  std::filesystem::path pluginPath;
  std::unordered_map<std::string, PluginInfo> plugins;

public:
  PluginLoader() : pluginPath(){};
  void loadPlugins(const std::string &path);
  void loadPlugin(const std::string &path);
  void unloadPlugins();
  void unloadPlugin(const std::string &name);
};

#endif
