#include "DamageSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"

DamageSystem::DamageSystem() { RequireComponent<BoxColliderComponent>(); }

void DamageSystem::SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
  eventBus->SubscribeToEvent(this, &DamageSystem::onCollision);
}

void DamageSystem::onProjectileHitPlayer(Entity &projectile, Entity &player) {
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

void DamageSystem::onProjectileHitEnemy(Entity &enemy, Entity &projectile) {
  auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
  if (projectileComponent.isFriendly) {
    HealthComponent &health = enemy.GetComponent<HealthComponent>();

    health.currentHealth -= projectileComponent.damage;
    if (health.currentHealth <= 0) {
      enemy.Kill();
    }
    projectile.Kill();
  }
}

void DamageSystem::onCollision(CollisionEvent &e) {
  Entity entity1 = e.entity1;
  Entity entity2 = e.entity2;

  if (entity1.BelongsGroup("projectile") && entity2.HasTag("player")) {
    onProjectileHitPlayer(entity1, entity2);
  } else if (entity1.HasTag("player") && entity2.BelongsGroup("projectile")) {
    onProjectileHitPlayer(entity2, entity1);
  }

  if (entity1.BelongsGroup("enemy") && entity2.BelongsGroup("projectile")) {
    onProjectileHitEnemy(entity1, entity2);
  } else if (entity1.BelongsGroup("projectile") &&
             entity2.BelongsGroup("enemy")) {
    onProjectileHitEnemy(entity2, entity1);
  }
}
