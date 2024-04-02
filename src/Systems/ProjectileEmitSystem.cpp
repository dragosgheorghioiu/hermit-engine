#include "ProjectileEmitSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

ProjectileEmitSystem::ProjectileEmitSystem() {
  RequireComponent<ProjectileEmitterComponent>();
  RequireComponent<TransformComponent>();
}

void ProjectileEmitSystem::SubscribeToEvents(
    std::unique_ptr<EventBus> &eventBus) {
  eventBus->SubscribeToEvent<KeyPressEvent>(
      this, &ProjectileEmitSystem::OnProjectileShoot);
}

glm::vec2 ProjectileEmitSystem::SetProjectileAngle(const float &angle,
                                                   const float &speed) {
  glm::vec2 velocity;
  velocity.x = cos(angle) * speed;
  velocity.y = -sin(angle) * speed;

  return velocity;
}

void ProjectileEmitSystem::OnProjectileShoot(KeyPressEvent &e) {
  if (e.keyCode == SDLK_SPACE) {
    for (Entity entity : GetSystemEntities()) {
      if (entity.HasTag("player")) {
        const ProjectileEmitterComponent emitter =
            entity.GetComponent<ProjectileEmitterComponent>();
        const TransformComponent transform =
            entity.GetComponent<TransformComponent>();

        // make prjectile come from the center of the player
        glm::vec2 position = transform.position;
        if (entity.HasComponent<SpriteComponent>()) {
          auto sprite = entity.GetComponent<SpriteComponent>();
          position.x +=
              static_cast<int>(transform.scale.x * sprite.srcRect.w / 2);
          position.y +=
              static_cast<int>(transform.scale.y * sprite.srcRect.h / 2);
        }

        // get the direction of the projectile
        glm::vec2 velocity =
            SetProjectileAngle(emitter.angle, emitter.velocity);

        CreateProjectile(entity.registry, entity, position, velocity,
                         emitter.duration, emitter.isFriendly, emitter.damage);
      }
    }
  }
}

void ProjectileEmitSystem::Update(std::unique_ptr<Registry> &registry) {
  for (Entity entity : GetSystemEntities()) {
    ProjectileEmitterComponent &emitter =
        entity.GetComponent<ProjectileEmitterComponent>();
    const TransformComponent &transform =
        entity.GetComponent<TransformComponent>();

    if (emitter.repeat == 0) {
      continue;
    }

    // spawn projectile
    if (SDL_GetTicks() - emitter.lastTime >
        static_cast<Uint32>(emitter.repeat)) {
      glm::vec2 position = transform.position;
      if (entity.HasComponent<SpriteComponent>()) {
        auto sprite = entity.GetComponent<SpriteComponent>();
        position.x +=
            static_cast<int>(transform.scale.x * sprite.srcRect.w / 2);
        position.y +=
            static_cast<int>(transform.scale.y * sprite.srcRect.h / 2);
      }

      // set direction of projectile
      glm::vec2 velocity = SetProjectileAngle(emitter.angle, emitter.velocity);

      CreateProjectile(registry.get(), entity, position, velocity,
                       emitter.duration, emitter.isFriendly, emitter.damage);

      emitter.lastTime = SDL_GetTicks();
    }
  }
}

void ProjectileEmitSystem::CreateProjectile(Registry *registry, Entity &entity,
                                            glm::vec2 position,
                                            glm::vec2 velocity, int duration,
                                            bool isFriendly, int damage) {
  Entity projectile = registry->CreateEntity();
  projectile.Group("projectile");
  projectile.AddComponent<TransformComponent>(position, glm::vec2(1, 1), 0);
  projectile.AddComponent<SpriteComponent>(4, 4, "projectile", 4);
  projectile.AddComponent<RigidBodyComponent>(velocity);
  projectile.AddComponent<BoxColliderComponent>(glm::vec2(0, 0),
                                                glm::vec2(4, 4));
  projectile.AddComponent<ProjectileComponent>(duration, isFriendly, damage);
}
