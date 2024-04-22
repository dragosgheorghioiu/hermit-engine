#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "Plugin/PluginInfo.h"
#include "PluginComponentFactory.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>
#include <unordered_map>

class PluginLoader {
private:
  std::filesystem::path pluginPath;
  std::unordered_map<std::string, PluginInfo> plugins;
  PluginComponentFactory componentFactory;

public:
  PluginLoader() = default;
  void loadSystems(const std::string &path);
  void loadSystem(const std::string &path);
  void loadComponents(const std::string &path);
  void loadComponent(const std::string &path);
  void unloadSystems();
  void unloadComponents();
  void unloadSystem(const std::string &name);
  void callSystemUpdate(const std::string &name, std::vector<void *> params);
  PluginComponentFactory getComponentFactory();
  void DestroySelf();
  ~PluginLoader();
};

#endif
