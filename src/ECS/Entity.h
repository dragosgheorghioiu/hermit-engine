#ifndef ENTITY_H
#define ENTITY_H

#include "Plugin/PluginComponentFactory.h"

class Entity {
private:
  int id;

public:
  Entity(int id) : id(id){};
  ~Entity() = default;
  int getId() const;

  bool operator==(const Entity &other) const { return id == other.id; }
  bool operator!=(const Entity &other) const { return id != other.id; }
  bool operator<=(const Entity &other) const { return id <= other.id; }
  bool operator>=(const Entity &other) const { return id >= other.id; }
  bool operator<(const Entity &other) const { return id < other.id; }
  bool operator>(const Entity &other) const { return id > other.id; }

  template <typename... TArgs>
  void addComponent(ComponentFactoryInfo componentInfo, TArgs &&...args);
  void removeComponent(ComponentInfo &componentInfo);
  ComponentInfo &getComponent(ComponentInfo &componentInfo);
  bool hasComponent(ComponentInfo &componentInfo);

  void kill();
  void tag(const std::string &tag);
  void removeTag();
  bool hasTag(const std::string &tag) const;

  void group(const std::string &group);
  void removeGroup(const std::string &group);
  bool belongsGroup(const std::string &group) const;

  class Registry *registry;
};

#endif
