#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
  Entity entity1;
  Entity entity2;
  CollisionEvent(Entity entity1, Entity entity2)
      : entity1(entity1), entity2(entity2) {}
};

#endif
