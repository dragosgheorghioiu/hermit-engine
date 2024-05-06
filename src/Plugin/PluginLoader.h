#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "../ECS/Registry.h"
#include "../Plugin/SystemInfo.h"
#include "PluginComponentFactory.h"
#include "PluginEventFactory.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>
#include <unordered_map>

class PluginLoader {
private:
  std::filesystem::path pluginPath;
  std::unordered_map<std::string, SystemInfo> plugins;
  PluginComponentFactory componentFactory;
	PluginEventFactory eventFactory;

public:
  PluginLoader() = default;
  void loadSystems(const std::string &path, RegistryType *registry);
  void loadSystem(const std::string &path, RegistryType *registry);
  void loadComponents(const std::string &path);
  void loadComponent(const std::string &path);
  void unloadSystems();
  void unloadComponents();
  void unloadSystem(const std::string &name);
  void callSystemUpdate(RegistryType *registry, const std::string &name,
                        std::vector<void *> params);
  PluginComponentFactory &getComponentFactory();
  ComponentFactoryInfo &getComponentInfo(const std::string &name);
  void DestroySelf();
  ~PluginLoader();
};

#endif
