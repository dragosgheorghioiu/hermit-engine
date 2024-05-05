#ifndef ENTITY_H
#define ENTITY_H

#include "../Plugin/PluginComponentFactory.h"

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

  class RegistryType *registry;
};

#endif
