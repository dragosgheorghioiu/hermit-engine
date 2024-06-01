#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class MovementSystem : public System {
public:
  MovementSystem();

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus);

  void onCollision(CollisionEvent &e);

  void OnEnemyObstacleCollision(Entity &enemy, Entity &obstacle);

  void Update(double dt);
};

#endif
