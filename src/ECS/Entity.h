#ifndef ENTITY_H
#define ENTITY_H

#include "../Plugin/PluginComponentFactory.h"
#include <sol/state.hpp>
class EntityType {
private:
  int id;

public:
  EntityType(int id) : id(id){};
  ~EntityType() = default;
  int getId() const;

  bool operator==(const EntityType &other) const { return id == other.id; }
  bool operator!=(const EntityType &other) const { return id != other.id; }
  bool operator<=(const EntityType &other) const { return id <= other.id; }
  bool operator>=(const EntityType &other) const { return id >= other.id; }
  bool operator<(const EntityType &other) const { return id < other.id; }
  bool operator>(const EntityType &other) const { return id > other.id; }

  void addComponent(
      ComponentFactoryInfo componentInfo,
      std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                               std::vector<bool>, std::vector<float>>>
          args);
  void removeComponent(ComponentInfo &componentInfo);
  ComponentInfo &getComponent(ComponentInfo &componentInfo);
  ComponentInfo &getComponent(std::string componentName);
  bool hasComponent(std::string componentName);
  bool hasComponent(ComponentInfo &componentInfo);
  bool hasComponent(int componentId);

  void kill();
  void tag(const std::string &tag);
  void removeTag();
  bool hasTag(const std::string &tag) const;

  void group(const std::string &group);
  void removeGroup(const std::string &group);
  bool belongsGroup(const std::string &group) const;

  static void createLuaUserType(sol::state &lua);

  class RegistryType *registry;
};

#endif
