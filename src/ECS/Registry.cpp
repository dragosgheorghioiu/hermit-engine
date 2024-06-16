#include "Registry.h"

EntityType RegistryType::createEntity() {
  int entityId = 0;
  if (!freeIds.empty()) {
    entityId = freeIds.back();
    freeIds.pop_back();
  } else {
    entityId = numEntities++;
  }
  EntityType entity(entityId);
  entity.registry = this;

  entitiesToBeAdded.insert(entity);

  if (entityId >= static_cast<int>(entityComponentSignatures.size())) {
    entityComponentSignatures.resize(entityId + 1);
  }

  // Logger::Log("Entity has been created with id = " +
  // std::to_string(entityId));

  return entity;
}

std::vector<EntityType> RegistryType::getAllEntities() {
  std::vector<EntityType> entities;
  // get all groups
  std::vector<std::string> groups = getAllGroups();
  // get all entities from each group
  for (auto &group : groups) {
    std::vector<EntityType> groupEntities = getEntitiesByGroup(group);
    entities.insert(entities.end(), groupEntities.begin(), groupEntities.end());
  }
  // get all tags
  std::vector<std::string> tags = getAllTags();
  // get all entities from each tag
  for (auto &tag : tags) {
    entities.push_back(getEntityByTag(tag));
  }

  return entities;
}

void RegistryType::destroyEntity(const EntityType &entity) {
  const auto entityId = entity.getId();
  entityComponentSignatures[entityId].reset();

  for (auto pool : pluginComponentPools) {
    if (pool)
      pool->RemoveEntityFromPool(entityId);
  }

  for (auto &system : pluginSystems) {
    system.second->instance->removeEntityFromSystem(entity);
  }
  freeIds.push_back(entityId);
  removeTagFromEntity(entity);
  removeEntityFromGroup(entity, groupPerEntity[entityId]);
}

void RegistryType::addEntityToBeDestroyed(const EntityType &entity) {
  entitiesToBeDestroyed.insert(entity);
}

void RegistryType::addTagToEntity(EntityType entity, const std::string &tag) {
  entityPerTag.insert({tag, entity});
  tagPerEntity.insert({entity.getId(), tag});
}

std::string RegistryType::getTagFromEntity(EntityType entity) const {
  std::string tag = "";
  if (tagPerEntity.find(entity.getId()) != tagPerEntity.end()) {
    tag = tagPerEntity.at(entity.getId());
  }
  return tag;
}

bool RegistryType::entityHasTag(EntityType entity,
                                const std::string &tag) const {
  return tagPerEntity.find(entity.getId()) != tagPerEntity.end();
}

EntityType RegistryType::getEntityByTag(const std::string &tag) {
  return entityPerTag.at(tag);
}

void RegistryType::removeTagFromEntity(EntityType entity) {
  const auto entityId = entity.getId();
  if (tagPerEntity.find(entityId) == tagPerEntity.end()) {
    return;
  }
  const auto tag = tagPerEntity.at(entityId);
  entityPerTag.erase(tag);
  tagPerEntity.erase(entityId);
}

void RegistryType::addGroupToEntity(EntityType entity,
                                    const std::string &group) {
  entitiesPerGroup.insert({group, std::set<EntityType>()});
  groupPerEntity.insert({entity.getId(), group});
  entitiesPerGroup[group].insert(entity);
}

bool RegistryType::entityBelongsGroup(EntityType entity,
                                      const std::string &group) const {
  if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
    return false;
  }
  return entitiesPerGroup.at(group).find(entity.getId()) !=
         entitiesPerGroup.at(group).end();
}

std::vector<EntityType>
RegistryType::getEntitiesByGroup(const std::string &group) {
  if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
    return std::vector<EntityType>();
  }
  std::vector<EntityType> entities;
  for (auto &entity : entitiesPerGroup[group]) {
    entities.push_back(EntityType(entity));
  }
  return entities;
}

std::string RegistryType::getGroupFromEntity(EntityType entity) const {
  std::string group = "";
  if (groupPerEntity.find(entity.getId()) != groupPerEntity.end()) {
    group = groupPerEntity.at(entity.getId());
  }
  return group;
}

std::vector<std::string> RegistryType::getAllGroups() const {
  std::vector<std::string> groups;
  for (auto &group : entitiesPerGroup) {
    groups.push_back(group.first);
  }
  return groups;
}

std::vector<std::string> RegistryType::getAllTags() const {
  std::vector<std::string> tags;
  for (auto &tag : tagPerEntity) {
    tags.push_back(tag.second);
  }
  return tags;
}

void RegistryType::removeEntityFromGroup(EntityType entity,
                                         const std::string &group) {
  const auto entityId = entity.getId();
  if (groupPerEntity.find(entityId) == groupPerEntity.end()) {
    return;
  }
  groupPerEntity.erase(entityId);
  if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
    return;
  }
  entitiesPerGroup[group].erase(entity);
}

void RegistryType::addPluginSystem(
    void *(*createInstance)(), const std::string &name,
    void (*destroyInstance)(void *), boost::dll::shared_library &library,
    const char **requiredComponents, const char **subscribedEvents,
    sol::state *lua, PluginEventFactory *eventFactory) {
  Logger::Log("Adding plugin system: " + name);
  std::unique_ptr<SystemInfo> systemInfo = std::make_unique<SystemInfo>(
      name, static_cast<SystemInstance *>(createInstance()), library,
      destroyInstance, requiredComponents);
  systemInfo->instance->setLua(lua);

  if (subscribedEvents != nullptr) {
    for (int i = 0; subscribedEvents[i] != nullptr; i++) {
      eventFactory->subscribe(subscribedEvents[i], systemInfo.get());
    }
  }

  pluginSystems.insert({name, std::move(systemInfo)});
}

void RegistryType::removePluginSystem(const std::string &name) {
  pluginSystems.erase(name);
}

SystemInfo *RegistryType::getPluginSystem(const std::string &name) {
  if (pluginSystems.find(name) == pluginSystems.end()) {
    Logger::Err("Plugin system not found: " + name);
    return nullptr;
  }
  return pluginSystems[name].get();
}

void RegistryType::callPluginSystemUpdate(const std::string &name,
                                          std::vector<void *> params) {
  SystemInfo *system = getPluginSystem(name);
  if (system == nullptr) {
    return;
  }
  system->instance->update(params);
}

bool RegistryType::hasPluginSystem(const std::string &name) const {
  return pluginSystems.find(name) != pluginSystems.end();
}

void RegistryType::addEntityToSystems(EntityType entity) {
  const auto entityId = entity.getId();
  const auto &entityComponentSignature = entityComponentSignatures[entityId];

  // Loop all the systems
  for (auto &system : pluginSystems) {
    const auto &systemComponentSignature =
        system.second->instance->getComponentSignature();

    // Check if the system is interested in the entity
    bool isInterested = (entityComponentSignature & systemComponentSignature) ==
                        systemComponentSignature;

    if (isInterested) {
      system.second->instance->addEntityToSystem(entity);
    }
  }
}

void RegistryType::update() {
  for (auto entity : entitiesToBeDestroyed) {
    destroyEntity(entity);
  }
  entitiesToBeDestroyed.clear();
  for (auto &entity : entitiesToBeAdded) {
    addEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();
}

void EntityType::removeComponent(ComponentInstance &componentInfo) {
  registry->removeComponentFromEntity(*this, componentInfo);
}

bool EntityType::hasComponent(ComponentInstance &componentInfo) {
  return registry->hasComponentFromEntity(*this, componentInfo);
}

bool EntityType::hasComponent(std::string componentName) {
  return registry->hasComponentFromEntity(*this, componentName);
}

bool EntityType::hasComponent(int componentId) {
  Logger::Debug("Checking for component: " + std::to_string(componentId));
  return registry->hasComponentFromEntity(*this, componentId);
}

ComponentInstance &EntityType::getComponent(ComponentInstance &componentInfo) {
  return registry->getComponentFromEntity(*this, componentInfo);
}

ComponentInstance &EntityType::getComponent(std::string componentName) {
  return registry->getComponentFromEntity(*this, componentName);
}

ComponentInstance &
RegistryType::getComponentFromEntity(const EntityType &entity,
                                     ComponentInstance &componentInfo) {
  const auto componentId = componentInfo.id;
  const auto entityId = entity.getId();

  std::shared_ptr<ComponentInstancePool> pool =
      pluginComponentPools[componentId];
  return pool->Get(entityId);
}

ComponentInstance &
RegistryType::getComponentFromEntity(const EntityType &entity,
                                     std::string componentName) {
  for (auto &component : pluginComponentPools) {
    if (component == nullptr)
      continue;
    if (component->GetName() == componentName) {
      return component->Get(entity.getId());
    }
  }
  Logger::Err("Component not found: " + componentName);
  exit(1);
}

bool RegistryType::hasComponentFromEntity(const EntityType &entity,
                                          std::string componentName) {
  for (auto &component : pluginComponentPools) {
    if (component == nullptr)
      continue;
    if (component->GetName() == componentName) {
      return component->Has(entity.getId());
    }
  }
  return false;
}

bool RegistryType::hasComponentFromEntity(const EntityType &entity,
                                          ComponentInstance &componentInfo) {
  const auto componentId = componentInfo.id;
  const auto entityId = entity.getId();

  return entityComponentSignatures[entityId].test(componentId);
}

bool RegistryType::hasComponentFromEntity(const EntityType &entity,
                                          int componentId) {
  const auto entityId = entity.getId();
  return entityComponentSignatures[entityId].test(componentId);
}

void RegistryType::removeComponentFromEntity(const EntityType &entity,
                                             ComponentInstance &componentInfo) {
  const auto entityId = entity.getId();
  const auto componentId = componentInfo.id;

  std::shared_ptr<ComponentInstancePool> pool =
      pluginComponentPools[componentId];
  pool->Remove(entityId);
  entityComponentSignatures[entityId].set(componentId, false);
}

void RegistryType::addComponentToEntity(
    const EntityType &entity, ComponentFactoryInfo &componentFactoryInfo,
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        args) {
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
    std::shared_ptr<ComponentInstancePool> newPool =
        std::make_shared<ComponentInstancePool>(entityId + 1,
                                                componentFactoryInfo.getName());
    Logger::Log("Creating new pool for component: " +
                componentFactoryInfo.getName());
    pluginComponentPools[componentId] = newPool;
  }

  std::shared_ptr<ComponentInstancePool> pool =
      pluginComponentPools[componentId];
  std::unique_ptr<ComponentInstance> componentInfo =
      componentFactoryInfo.createComponent(args);
  pool->Set(entityId, std::move(componentInfo));
  entityComponentSignatures[entityId].set(componentId);
}

void EntityType::addComponent(
    ComponentFactoryInfo componentInfo,
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        args) {
  registry->addComponentToEntity(*this, componentInfo, args);
}

void RegistryType::createLuaUserType(sol::state &lua) {
  sol::usertype<RegistryType> registry = lua.new_usertype<RegistryType>(
      "registry", "create_entity", &RegistryType::createEntity,
      "destroy_entity", &RegistryType::destroyEntity,
      "add_entity_to_be_destroyed", &RegistryType::addEntityToBeDestroyed,
      "add_tag_to_entity", &RegistryType::addTagToEntity, "entity_has_tag",
      &RegistryType::entityHasTag);
  registry.set_function("get_tag_from_entity", &RegistryType::getTagFromEntity);
  registry.set_function("get_entity_by_tag", &RegistryType::getEntityByTag);
  registry.set_function("remove_tag_from_entity",
                        &RegistryType::removeTagFromEntity);
  registry.set_function("add_group_to_entity", &RegistryType::addGroupToEntity);
  registry.set_function("entity_belongs_group",
                        &RegistryType::entityBelongsGroup);
  registry.set_function("get_entities_by_group",
                        &RegistryType::getEntitiesByGroup);
  registry.set_function("remove_entity_from_group",
                        &RegistryType::removeEntityFromGroup);
  registry.set_function("add_component_to_entity",
                        &RegistryType::addComponentToEntity);
  registry.set_function("remove_component_from_entity",
                        &RegistryType::removeComponentFromEntity);
  registry.set_function(
      "has_component_from_entity",
      sol::overload(sol::resolve<bool(const EntityType &, ComponentInstance &)>(
                        &RegistryType::hasComponentFromEntity),
                    sol::resolve<bool(const EntityType &, std::string)>(
                        &RegistryType::hasComponentFromEntity),
                    sol::resolve<bool(const EntityType &, int)>(
                        &RegistryType::hasComponentFromEntity)));
  registry.set_function(
      "get_component_from_entity",
      sol::overload(
          sol::resolve<ComponentInstance &(const EntityType &,
                                           ComponentInstance &)>(
              &RegistryType::getComponentFromEntity),
          sol::resolve<ComponentInstance &(const EntityType &, std::string)>(
              &RegistryType::getComponentFromEntity)));
  registry.set_function("add_plugin_system", &RegistryType::addPluginSystem);
  registry.set_function("remove_plugin_system",
                        &RegistryType::removePluginSystem);
  registry.set_function("get_plugin_system", &RegistryType::getPluginSystem);
  registry.set_function("call_plugin_system_update",
                        &RegistryType::callPluginSystemUpdate);
  registry.set_function("has_plugin_system", &RegistryType::hasPluginSystem);
}

void RegistryType::clear() {
  for (auto &pool : pluginComponentPools) {
    if (pool)
      pool->Clear();
  }
  for (auto &system : pluginSystems) {
    system.second->instance->removeAllEntitiesFromSystem();
  }
  entityComponentSignatures.clear();
  entitiesToBeAdded.clear();
  entitiesToBeDestroyed.clear();
  entityPerTag.clear();
  tagPerEntity.clear();
  entitiesPerGroup.clear();
  groupPerEntity.clear();
  pluginComponentPools.clear();
  freeIds.clear();
  numEntities = 0;
}

void RegistryType::printFreeIds() {
  Logger::Warn("Free ids:");
  for (auto &id : freeIds) {
    Logger::Warn("Free id: " + std::to_string(id));
  }
}

Signature
RegistryType::getComponentSignatureFromEntity(const EntityType &entity) const {
  return entityComponentSignatures[entity.getId()];
}
