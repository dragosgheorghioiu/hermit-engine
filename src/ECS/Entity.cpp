#include "Entity.h"
#include "Registry.h"

int EntityType::getId() const { return id; }

void EntityType::kill() {
  // Logger::Log("Entity with id = " + std::to_string(id) + " has been killed");
  registry->addEntityToBeDestroyed(*this);
}

void EntityType::tag(const std::string &tag) {
  registry->addTagToEntity(*this, tag);
}
void EntityType::removeTag() { registry->removeTagFromEntity(*this); }
bool EntityType::hasTag(const std::string &tag) const {
  return registry->entityHasTag(*this, tag);
}

void EntityType::group(const std::string &group) {
  registry->addGroupToEntity(*this, group);
}
void EntityType::removeGroup(const std::string &group) {
  registry->removeEntityFromGroup(*this, group);
}
bool EntityType::belongsGroup(const std::string &group) const {
  return registry->entityBelongsGroup(*this, group);
}
