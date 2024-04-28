#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <string>
#include <unordered_map>

struct ComponentInfo {
  int id;
  std::string path;
  std::string name;
  void *(*createInstance)(...);
  void (*destroyInstance)(void *);
  void *instance;
};

class PluginComponentFactory {
private:
  int size;
  std::unordered_map<std::string, ComponentInfo> components;

public:
  PluginComponentFactory() = default;
  ~PluginComponentFactory() = default;

  void loadComponents(const std::string &path);
  void loadComponent(const std::string &path, int id);
  void unloadComponents();
  void unloadComponent(const std::string &name);

  ComponentInfo getComponentInfo(const std::string &name);

  int getSize() { return size; }
};

#endif
