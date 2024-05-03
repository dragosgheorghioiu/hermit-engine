#ifndef REGISTRY_H
#define REGISTRY_H

#include "ECS/ComponentInfoPool.h"
#include "ECS/Entity.h"
#include "Plugin/PluginComponentFactory.h"
#include "Plugin/SystemInfo.h"
#include <deque>
#include <set>

class Registry {
private:
  int numEntities = 0;

  // sets of entities that are marked to be removed at update
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeDestroyed;

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
  std::unordered_map<std::string, Entity> entityPerTag;
  std::unordered_map<int, std::string> tagPerEntity;

  // Entity groups duplicated for fast access both ways
  std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
  std::unordered_map<int, std::string> groupPerEntity;

public:
  Registry() { Logger::Log("Registry constructor"); }
  ~Registry() { Logger::Log("Registry destructor"); }

  Entity CreateEntity();
  void AddEntityToBeDestroyed(const Entity &entity);
  void DestroyEntity(const Entity &entity);

  // Tags
  void AddTagToEntity(Entity entity, const std::string &tag);
  bool EntityHasTag(Entity entity, const std::string &tag) const;
  Entity GetEntityByTag(const std::string &tag) const;
  void RemoveTagFromEntity(Entity entity);

  // Groups
  void AddGroupToEntity(Entity entity, const std::string &group);
  bool EntityBelongsGroup(Entity entity, const std::string &group) const;
  std::set<Entity> GetEntitiesByGroup(const std::string &group) const;
  void RemoveEntityFromGroup(Entity entity, const std::string &group);

  template <typename... TArgs>
  void addComponentToEntity(const Entity &entity,
                            ComponentFactoryInfo &componentInfo,
                            TArgs &&...args);
  void removeComponentFromEntity(const Entity &entity,
                                 ComponentInfo &componentInfo);
  ComponentInfo &getComponentFromEntity(const Entity &entity,
                                        ComponentInfo &componentInfo);
  bool hasComponentFromEntity(const Entity &entity,
                              ComponentInfo &componentInfo);

  void addPluginSystem(std::unique_ptr<SystemInfo> systemInfo);
  void removePluginSystem(const std::string &name);
  SystemInfo &getPluginSystem(const std::string &name);
  bool hasPluginSystem(const std::string &name) const;
  void callPluginSystemUpdate(const std::string &name,
                              std::vector<void *> params);

  void AddEntityToSystems(Entity entity);

  void Update();
};

template <typename... TArgs>
void Registry::addComponentToEntity(const Entity &entity,
                                    ComponentFactoryInfo &componentFactoryInfo,
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
        std::make_shared<ComponentInfoPool>(entityId + 1);
    pluginComponentPools[componentId] = newPool;
  }

  std::shared_ptr<ComponentInfoPool> pool = pluginComponentPools[componentId];
  std::unique_ptr<ComponentInfo> componentInfo =
      componentFactoryInfo.createComponent(std::forward<TArgs>(args)...);
  pool->Set(entityId, std::move(componentInfo));
  entityComponentSignatures[entityId].set(componentId);
}

#endif
