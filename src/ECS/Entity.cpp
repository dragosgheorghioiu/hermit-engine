#include "Entity.h"
#include "Registry.h"

int Entity::getId() const { return id; }

void Entity::kill() {
  // Logger::Log("Entity with id = " + std::to_string(id) + " has been killed");
  registry->AddEntityToBeDestroyed(*this);
}
