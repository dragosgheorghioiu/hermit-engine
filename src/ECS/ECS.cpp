#include "ECS.h"
#include "../Logger/Logger.h"
#include <algorithm>
#include <string>

int IComponent::nextId = 0;

int Entity::GetId() const { return id; }

void System::AddEntityToSystem(const Entity &entity) {
  entities.push_back(entity);
}
void System::RemoveEntityFromSystem(const Entity &entity) {
  entities.erase(
      std::remove_if(entities.begin(), entities.end(),
                     [&entity](Entity other) { return entity == other; }),
      entities.end());
}
std::vector<Entity> System::GetSystemEntities() const { return entities; }
const Signature &System::GetComponentSignature() const {
  return componentSignature;
}

Entity Registry::CreateEntity() {
  int entityId = 0;
  if (!freeIds.empty()) {
    entityId = freeIds.back();
    freeIds.pop_back();
  } else {
    entityId = numEntities++;
  }
  Entity entity(entityId);
  entity.registry = this;

  entitiesToBeAdded.insert(entity);

  if (entityId >= static_cast<int>(entityComponentSignatures.size())) {
    entityComponentSignatures.resize(entityId + 1);
  }

  Logger::Log("Entity has been created with id = " + std::to_string(entityId));

  return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
  const auto entityId = entity.GetId();
  const auto &entityComponentSignature = entityComponentSignatures[entityId];

  // Loop all the systems
  for (auto &system : systems) {
    const auto &systemComponentSignature =
        system.second->GetComponentSignature();

    bool isInterested = (entityComponentSignature & systemComponentSignature) ==
                        systemComponentSignature;

    if (isInterested) {
      system.second->AddEntityToSystem(entity);
    }
  }
}

void Registry::Update() {
  for (auto &entity : entitiesToBeAdded) {
    AddEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();

  for (auto entity : entitiesToBeDestroyed) {
    DestroyEntity(entity);
  }
  entitiesToBeDestroyed.clear();
}

void Registry::DestroyEntity(const Entity &entity) {
  const auto entityId = entity.GetId();
  entityComponentSignatures[entityId].reset();

  for (auto &system : systems) {
    system.second->RemoveEntityFromSystem(entity);
  }
  freeIds.push_back(entityId);
}

void Entity::Kill() {
  Logger::Log("Entity with id = " + std::to_string(id) + " has been killed");
  registry->AddEntityToBeDestroyed(*this);
}

void Registry::AddEntityToBeDestroyed(const Entity &entity) {
  entitiesToBeDestroyed.insert(entity);
}

void Registry::AddTagToEntity(Entity entity, const std::string &tag) {
  entityPerTag.insert({tag, entity});
  tagPerEntity.insert({entity.GetId(), tag});
}

bool Registry::EntityHasTag(Entity entity, const std::string &tag) const {
  return tagPerEntity.find(entity.GetId()) != tagPerEntity.end();
}

Entity Registry::GetEntityByTag(const std::string &tag) const {
  return entityPerTag.at(tag);
}

void Registry::RemoveTagFromEntity(Entity entity) {
  const auto entityId = entity.GetId();
  const auto tag = tagPerEntity.at(entityId);
  entityPerTag.erase(tag);
  tagPerEntity.erase(entityId);
}

void Registry::AddGroupToEntity(Entity entity, const std::string &group) {
  entitiesPerGroup.insert({group, std::set<Entity>()});
  groupPerEntity.insert({entity.GetId(), group});
  entitiesPerGroup[group].insert(entity);
}

bool Registry::EntityBelongsGroup(Entity entity,
                                  const std::string &group) const {
  return entitiesPerGroup.at(group).find(entity.GetId()) !=
         entitiesPerGroup.at(group).end();
}

std::set<Entity> Registry::GetEntitiesByGroup(const std::string &group) const {
  return entitiesPerGroup.at(group);
}

void Registry::RemoveEntityFromGroup(Entity entity, const std::string &group) {
  const auto entityId = entity.GetId();
  groupPerEntity.erase(entityId);
  entitiesPerGroup[group].erase(entity);
}

void Entity::Tag(const std::string &tag) {
  registry->AddTagToEntity(*this, tag);
}
void Entity::RemoveTag() { registry->RemoveTagFromEntity(*this); }
bool Entity::HasTag(const std::string &tag) const {
  return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string &group) {
  registry->AddGroupToEntity(*this, group);
}
void Entity::RemoveGroup(const std::string &group) {
  registry->RemoveEntityFromGroup(*this, group);
}
bool Entity::BelongsGroup(const std::string &group) const {
  return registry->EntityBelongsGroup(*this, group);
}
