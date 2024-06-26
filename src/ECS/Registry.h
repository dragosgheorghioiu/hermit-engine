#ifndef REGISTRY_H
#define REGISTRY_H

#include "../Plugin/PluginComponentFactory.h"
#include "../Plugin/PluginEventFactoryList.h"
#include "../Plugin/SystemInfo.h"
#include "ComponentInfoPool.h"
#include <deque>
#include <set>

class RegistryType {
private:
  int numEntities = 0;

  // sets of entities that are marked to be removed at update
  std::set<EntityType> entitiesToBeAdded;
  std::set<EntityType> entitiesToBeDestroyed;

  // vector of pools
  // each pool contains all the data for a certain component type
  // componentsPool index is a component id
  // Pool index is a entity id
  std::vector<std::shared_ptr<ComponentInstancePool>> pluginComponentPools;

  // Vector of components signatures per entity
  // Vector index is entity id
  std::vector<Signature> entityComponentSignatures;

  // vector of systems
  std::unordered_map<std::string, std::unique_ptr<SystemInfo>> pluginSystems;

  // deque of freeIds
  std::deque<int> freeIds;

  // Entity tags duplicated for fast access both ways
  std::unordered_map<std::string, EntityType> entityPerTag;
  std::unordered_map<int, std::string> tagPerEntity;

  // Entity groups duplicated for fast access both ways
  std::unordered_map<std::string, std::set<EntityType>> entitiesPerGroup;
  std::unordered_map<int, std::string> groupPerEntity;

public:
  RegistryType();
  ~RegistryType();

  void clear();

  EntityType createEntity();
  void addEntityToBeDestroyed(const EntityType &entity);
  void destroyEntity(const EntityType &entity);
  void killAllEntities();

  // Tags
  void addTagToEntity(EntityType entity, const std::string &tag);
  bool entityHasTag(EntityType entity, const std::string &tag) const;
  EntityType getEntityByTag(const std::string &tag);
  void removeTagFromEntity(EntityType entity);
  std::string getTagFromEntity(EntityType entity) const;
  std::vector<std::string> getAllTags() const;

  // Groups
  void addGroupToEntity(EntityType entity, const std::string &group);
  bool entityBelongsGroup(EntityType entity, const std::string &group) const;
  std::vector<EntityType> getEntitiesByGroup(const std::string &group);
  void removeEntityFromGroup(EntityType entity, const std::string &group);
  std::vector<std::string> getAllGroups() const;
  std::string getGroupFromEntity(EntityType entity) const;

  std::vector<EntityType> getAllEntities();

  void addComponentToEntity(
      const EntityType &entity, ComponentFactoryInfo &componentInfo,
      std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                               std::vector<bool>, std::vector<float>>>
          args);
  void removeComponentFromEntity(const EntityType &entity,
                                 ComponentInstance &componentInfo);
  ComponentInstance &getComponentFromEntity(const EntityType &entity,
                                            ComponentInstance &componentInfo);
  ComponentInstance &getComponentFromEntity(const EntityType &entity,
                                            std::string componentName);
  bool hasComponentFromEntity(const EntityType &entity,
                              ComponentInstance &componentInfo);
  bool hasComponentFromEntity(const EntityType &entity,
                              std::string componentName);
  bool hasComponentFromEntity(const EntityType &entity, int componentId);
  void addPluginSystem(void *(*createInstance)(), const std::string &name,
                       void (*destroyInstance)(void *),
                       boost::dll::shared_library &library,
                       const char **requiredComponents,
                       const char **subscribedEvents, sol::state *lua,
                       PluginEventFactoryList *eventFactory);
  void removePluginSystem(const std::string &name);
  SystemInfo *getPluginSystem(const std::string &name);
  bool hasPluginSystem(const std::string &name) const;
  void callPluginSystemUpdate(const std::string &name,
                              std::vector<void *> params);

  Signature getComponentSignatureFromEntity(const EntityType &entity) const;

  void addEntityToSystems(EntityType entity);

  void update();

  void printFreeIds();

  static void createLuaUserType(sol::state &lua);
};

#endif
