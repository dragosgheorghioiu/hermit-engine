#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"

class CollisionSystem : public System {
public:
  CollisionSystem();

  void Update(std::unique_ptr<EventBus> &eventBus);
};

#endif
