#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
public:
  DamageSystem() { RequireComponent<BoxColliderComponent>(); }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent(this, &DamageSystem::onCollision);
  }

  void onCollision(CollisionEvent &e) {
    Logger::Log("The damage system is handling a collision event");
    e.entity1.Kill();
    e.entity2.Kill();
  }

  void Update() {}
};

#endif
