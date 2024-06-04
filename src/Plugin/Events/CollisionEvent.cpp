#include "CollisionEvent.h"
#include <any>

extern "C" void *createInstance(std::vector<std::any> args) {
  EntityType entity1 = std::any_cast<EntityType>(args[0]);
  EntityType entity2 = std::any_cast<EntityType>(args[1]);
  return static_cast<void *>(new CollisionEvent(entity1, entity2));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<CollisionEvent *>(instance);
}
extern "C" const char *getEventName() { return "collisionEvent"; }
