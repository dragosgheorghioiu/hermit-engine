#include "Entity.h"
#include "Registry.h"
#include <sol/forward.hpp>

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

void EntityType::createLuaUserType(sol::state &lua) {
  sol::usertype<EntityType> entityType = lua.new_usertype<EntityType>(
      "entity_type", "kill", &EntityType::kill, "tag", &EntityType::tag,
      "remove_tag", &EntityType::removeTag, "has_tag", &EntityType::hasTag,
      "group", &EntityType::group, "remove_group", &EntityType::removeGroup,
      "belongs_group", &EntityType::belongsGroup);

  entityType.set_function("get_id", &EntityType::getId);
  entityType.set_function("remove_component", &EntityType::removeComponent);
  entityType.set_function(
      "has_component",
      sol::overload(
          sol::resolve<bool(std::string)>(&EntityType::hasComponent),
          sol::resolve<bool(ComponentInfo &)>(&EntityType::hasComponent)));
  entityType.set_function(
      "get_component",
      sol::overload(
          sol::resolve<ComponentInfo &(std::string)>(&EntityType::getComponent),
          sol::resolve<ComponentInfo &(ComponentInfo &)>(
              &EntityType::getComponent)));
  entityType.set_function("add_component", &EntityType::addComponent);
}
