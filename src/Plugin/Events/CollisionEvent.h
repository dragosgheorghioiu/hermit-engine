#ifndef DEMO_EVENT_H
#define DEMO_EVENT_H

#include "../../ECS/Entity.h"

struct CollisionEvent {
  EntityType entity1;
  EntityType entity2;

  CollisionEvent(EntityType entity1, EntityType entity2)
      : entity1(entity1), entity2(entity2) {}
  ~CollisionEvent() {}
};

#endif
