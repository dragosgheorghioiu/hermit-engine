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

bool RegistryType::entityHasTag(EntityType entity,
                                const std::string &tag) const {
  return tagPerEntity.find(entity.getId()) != tagPerEntity.end();
}

EntityType RegistryType::getEntityByTag(const std::string &tag) const {
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

std::set<EntityType>
RegistryType::getEntitiesByGroup(const std::string &group) const {
  return entitiesPerGroup.at(group);
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

void RegistryType::addPluginSystem(void *(*createInstance)(),
                                   const std::string &name,
                                   void (*destroyInstance)(void *),
                                   boost::dll::shared_library &library,
                                   const char **requiredComponents) {
  Logger::Log("Adding plugin system: " + name);
  pluginSystems.insert(
      {name, std::make_unique<SystemInfo>(
                 name, static_cast<SystemInstance *>(createInstance()), library,
                 destroyInstance, requiredComponents)});
}

void RegistryType::removePluginSystem(const std::string &name) {
  pluginSystems.erase(name);
}

SystemInfo &RegistryType::getPluginSystem(const std::string &name) {
  if (pluginSystems.find(name) == pluginSystems.end()) {
    Logger::Err("Plugin system not found: " + name);
    exit(1);
  }
  return *pluginSystems.at(name);
}

void RegistryType::callPluginSystemUpdate(const std::string &name,
                                          std::vector<void *> params) {
  getPluginSystem(name).instance->update(params);
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

    bool isInterested = (entityComponentSignature & systemComponentSignature) ==
                        systemComponentSignature;

    if (isInterested) {
      system.second->instance->addEntityToSystem(entity);
    }
  }
}

void RegistryType::update() {
  for (auto &entity : entitiesToBeAdded) {
    addEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();

  for (auto entity : entitiesToBeDestroyed) {
    destroyEntity(entity);
  }
  entitiesToBeDestroyed.clear();
}
