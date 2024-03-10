#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
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
    Entity entity1 = e.entity1;
    Entity entity2 = e.entity2;

    Logger::Log("entity1 is player: " +
                std::to_string(entity1.HasTag("player")));
    Logger::Log("entity2 is player: " +
                std::to_string(entity2.HasTag("player")));
    if (entity1.BelongsGroup("projectile") && entity2.HasTag("player")) {
      OnProjectileHitPlayer(entity1, entity2);
    } else if (entity1.HasTag("player") && entity2.BelongsGroup("projectile")) {
      OnProjectileHitPlayer(entity2, entity1);
    }
    //
    // if (entity1.HasTag("player") && entity2.BelongsGroup("projectile")) {
    //   OnProjectileHitPlayer(entity2, entity1);
    // }
    //
    // if (entity1.HasTag("player") && entity2.BelongsGroup("enemy")) {
    //   // OnEnemyHitPlayer(entity1, entity2);
    // }
    //
    // if (entity1.BelongsGroup("enemy") && entity2.BelongsGroup("projectile"))
    // {
    //   // OnEnemyHitByProjectile(entity1, entity2);
    // }
  }

  void OnProjectileHitPlayer(Entity &projectile, Entity &player) {
    auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
    if (!projectileComponent.isFriendly) {
      HealthComponent &health = player.GetComponent<HealthComponent>();

      health.currentHealth -= projectileComponent.damage;
      if (health.currentHealth <= 0) {
        player.Kill();
      }
      projectile.Kill();
    }
  }

  void Update() {}
};

#endif
