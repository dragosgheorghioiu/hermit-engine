#ifndef REGISTRY_H
#define REGISTRY_H

#include "../Plugin/PluginComponentFactory.h"
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
  std::vector<std::shared_ptr<ComponentInfoPool>> pluginComponentPools;

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
  RegistryType() { Logger::Log("Registry constructor"); }
  ~RegistryType() { Logger::Log("Registry destructor"); }

  EntityType createEntity();
  void addEntityToBeDestroyed(const EntityType &entity);
  void destroyEntity(const EntityType &entity);

  // Tags
  void addTagToEntity(EntityType entity, const std::string &tag);
  bool entityHasTag(EntityType entity, const std::string &tag) const;
  EntityType getEntityByTag(const std::string &tag) const;
  void removeTagFromEntity(EntityType entity);

  // Groups
  void addGroupToEntity(EntityType entity, const std::string &group);
  bool entityBelongsGroup(EntityType entity, const std::string &group) const;
  std::set<EntityType> getEntitiesByGroup(const std::string &group) const;
  void removeEntityFromGroup(EntityType entity, const std::string &group);

  template <typename... TArgs>
  void addComponentToEntity(const EntityType &entity,
                            ComponentFactoryInfo &componentInfo,
                            TArgs &&...args);
  void removeComponentFromEntity(const EntityType &entity,
                                 ComponentInfo &componentInfo);
  ComponentInfo &getComponentFromEntity(const EntityType &entity,
                                        ComponentInfo &componentInfo);
  ComponentInfo &getComponentFromEntity(const EntityType &entity,
                                        std::string componentName);
  bool hasComponentFromEntity(const EntityType &entity,
                              ComponentInfo &componentInfo);
  bool hasComponentFromEntity(const EntityType &entity,
                              std::string componentName);
  void addPluginSystem(void *(*createInstance)(), const std::string &name,
                       void (*destroyInstance)(void *),
                       boost::dll::shared_library &library,
                       const char **requiredComponents);
  void removePluginSystem(const std::string &name);
  SystemInfo &getPluginSystem(const std::string &name);
  bool hasPluginSystem(const std::string &name) const;
  void callPluginSystemUpdate(const std::string &name,
                              std::vector<void *> params);

  void addEntityToSystems(EntityType entity);

  void update();
};

template <typename... TArgs>
void RegistryType::addComponentToEntity(
    const EntityType &entity, ComponentFactoryInfo &componentFactoryInfo,
    TArgs &&...args) {
  const auto entityId = entity.getId();
  const auto componentId = componentFactoryInfo.getId();

  if (componentId >= (pluginComponentPools.size())) {
    pluginComponentPools.resize(componentId + 1);
  }

  if (pluginComponentPools.size() == 0) {
    Logger::Err("NO COMPONENT POOLS");
    exit(1);
  }

  if (pluginComponentPools[componentId] == nullptr) {
    std::shared_ptr<ComponentInfoPool> newPool =
        std::make_shared<ComponentInfoPool>(entityId + 1,
                                            componentFactoryInfo.getName());
    Logger::Log("Creating new pool for component: " +
                componentFactoryInfo.getName());
    pluginComponentPools[componentId] = newPool;
  }

  std::shared_ptr<ComponentInfoPool> pool = pluginComponentPools[componentId];
  std::unique_ptr<ComponentInfo> componentInfo =
      componentFactoryInfo.createComponent(std::forward<TArgs>(args)...);
  pool->Set(entityId, std::move(componentInfo));
  entityComponentSignatures[entityId].set(componentId);
}

template <typename... TArgs>
void EntityType::addComponent(ComponentFactoryInfo componentInfo,
                              TArgs &&...args) {
  registry->addComponentToEntity(*this, componentInfo,
                                 std::forward<TArgs>(args)...);
}

#endif
