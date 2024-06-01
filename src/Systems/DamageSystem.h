#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
private:
  void onProjectileHitPlayer(Entity &projectile, Entity &player);
  void onProjectileHitEnemy(Entity &projectile, Entity &enemy);

public:
  DamageSystem();

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus);

  void onCollision(CollisionEvent &e);

  void Update();
};

#endif
