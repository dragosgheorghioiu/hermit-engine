#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "../ECS/Registry.h"
#include "PluginComponentFactory.h"
#include "PluginEventFactory.h"
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>

class PluginLoader {
private:
  std::filesystem::path systemPath;
  std::unordered_map<std::string, std::string> systemNamesPaths;
  ComponentFactoryList componentFactoryList;
  PluginEventFactory eventFactory;

public:
  PluginLoader() = default;
  void loadSystems(const std::string &path, RegistryType *registry,
                   sol::state *lua);
  void loadSystem(const std::string &path, RegistryType *registry,
                  sol::state *lua);
  void loadComponents(const std::string &path, sol::state &lua);
  void loadComponent(const std::string &path, sol::state &lua);
  void loadEvents(const std::string &path);
  void loadEvent(const std::string &path);

  void unloadSystems(RegistryType *registry);
  void unloadComponents();
  void unloadEvents();

  void unloadSystem(RegistryType *registry, const std::string &name);
  void callSystemUpdate(RegistryType *registry, const std::string &name,
                        std::vector<void *> params);
  std::vector<std::string> getSystemsNamesList();
  ComponentFactoryList &getComponentFactory();
  PluginEventFactory &getEventFactory();
  // get the unordered map of system names and paths
  std::unordered_map<std::string, std::string> &getSystemNamesPaths();
  ComponentFactoryInfo &getComponentInfo(const std::string &name);
  void DestroySelf();
  ~PluginLoader();
};

#endif
