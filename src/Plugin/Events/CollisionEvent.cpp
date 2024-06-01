#include "CollisionEvent.h"

extern "C" void *createInstance(EntityType entity1, EntityType entity2) {
  return static_cast<void *>(new CollisionEvent(entity1, entity2));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<CollisionEvent *>(instance);
}
extern "C" const char *getEventName() { return "collisionEvent"; }
