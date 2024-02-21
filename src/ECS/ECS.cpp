#include "ECS.h"
#include "../Logger/Logger.h"
#include <algorithm>
#include <string>

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
std::vector<Entity> System::GetSystemEntites() const { return entities; }
const Signature &System::GetComponentSignature() const {
  return componentSignature;
}

Entity Registry::CreateEntity() {
  int entityId = numEntities++;
  Entity entity(entityId);

  entitiesToBeAdded.insert(entity);

  Logger::Log("Entity has been created with id = " + std::to_string(entityId));

  return entity;
}
