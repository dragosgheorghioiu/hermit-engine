#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "../Logger/Logger.h"
#include <boost/dll/shared_library.hpp>
#include <memory>
#include <sol/forward.hpp>
#include <sol/state.hpp>
#include <string>
#include <unordered_map>

struct ComponentInstance {
  std::string name;
  int id;
  void *instance;
  void (*destroyInstance)(void *);
  boost::dll::shared_library library;

  ComponentInstance(std::string name = "", int id = -1,
                    void *instance = nullptr,
                    void (*destroyInstance)(void *) = nullptr)
      : name(name), id(id), instance(instance),
        destroyInstance(destroyInstance) {}
  ~ComponentInstance() {
    if (!instance) {
      Logger::Err("Component instance is null");
      return;
    }
    if (!destroyInstance) {
      Logger::Err("Destroy instance function is null");
      return;
    }
    destroyInstance(instance);
  }

  static void createLuaUserType(sol::state &lua);
};

class ComponentFactoryInfo {
private:
  int id;
  std::string path;
  std::string name;
  void *(*createInstance)(...);
  void (*destroyInstance)(void *);
  boost::dll::shared_library library;

public:
  ComponentFactoryInfo(
      int id = -1, std::string path = "", std::string name = "",
      void *(*createInstance)(...) = nullptr,
      void (*destroyInstance)(void *) = nullptr,
      boost::dll::shared_library library = boost::dll::shared_library())
      : id(id), path(path), name(name), createInstance(createInstance),
        destroyInstance(destroyInstance), library(library) {}

  int getId() { return id; }
  std::string getPath() { return path; }
  std::string getName() { return name; }
  boost::dll::shared_library &getLibrary() { return library; }
  void *(*getCreateInstance())(...) { return createInstance; }
  void (*getDestroyInstance())(void *) { return destroyInstance; }

  template <typename... args_t>
  std::unique_ptr<ComponentInstance> createComponent(args_t &&...args) {
    void *instance = createInstance(std::forward<args_t>(args)...);
    return std::make_unique<ComponentInstance>(name, id, instance,
                                               destroyInstance);
  }
  ~ComponentFactoryInfo() {
    if (library.is_loaded()) {
      library.unload();
    }
  }
};

class ComponentFactoryList {
private:
  int size;
  std::unordered_map<std::string, ComponentFactoryInfo> components;

public:
  ComponentFactoryList() = default;
  ~ComponentFactoryList() = default;

  void loadComponents(const std::string &path, sol::state &lua);
  void loadComponent(const std::string &path, int id, sol::state &lua);
  void unloadComponents();
  void unloadComponent(const std::string &name);

  ComponentFactoryInfo &getComponentFactoryInfo(const std::string &name);

  int getSize() { return size; }
  void clear();
};

#endif
