#ifndef PROJECTILE_EMIT_SYSTEM_H
#define PROJECTILE_EMIT_SYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardPressEvent.h"

class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem() {
    RequireComponent<ProjectileEmitterComponent>();
    RequireComponent<TransformComponent>();
  }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressEvent>(
        this, &ProjectileEmitSystem::OnProjectileShoot);
  }

  glm::vec2 SetProjectileDirection(Direction direction, glm::vec2 velocity) {
    int directionX = 0;
    int directionY = 0;

    switch (direction) {
    case UP:
      directionY = -1;
      break;
    case RIGHT:
      directionX = 1;
      break;
    case DOWN:
      directionY = 1;
      break;
    case LEFT:
      directionX = -1;
      break;
    }

    velocity.x = directionX * velocity.x;
    velocity.y = directionY * velocity.y;

    return velocity;
  }

  void OnProjectileShoot(KeyPressEvent &e) {
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
              SetProjectileDirection(emitter.direction, emitter.velocity);

          CreateProjectile(entity.registry, entity, position, velocity,
                           emitter.duration, emitter.isFriendly,
                           emitter.damage);
        }
      }
    }
  }

  void Update(std::unique_ptr<Registry> &registry) {
    for (Entity entity : GetSystemEntities()) {
      ProjectileEmitterComponent &emitter =
          entity.GetComponent<ProjectileEmitterComponent>();
      const TransformComponent transform =
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
        glm::vec2 velocity =
            SetProjectileDirection(emitter.direction, emitter.velocity);

        CreateProjectile(registry.get(), entity, position, velocity,
                         emitter.duration, emitter.isFriendly, emitter.damage);

        emitter.lastTime = SDL_GetTicks();
      }
    }
  }

  void CreateProjectile(Registry *registry, Entity &entity, glm::vec2 position,
                        glm::vec2 velocity, int duration, bool isFriendly,
                        int damage) {
    Entity projectile = registry->CreateEntity();
    projectile.Group("projectile");
    projectile.AddComponent<TransformComponent>(position, glm::vec2(1, 1), 0);
    projectile.AddComponent<SpriteComponent>(4, 4, "projectile", 4);
    projectile.AddComponent<RigidBodyComponent>(velocity);
    projectile.AddComponent<BoxColliderComponent>(glm::vec2(0, 0),
                                                  glm::vec2(4, 4));
    projectile.AddComponent<ProjectileComponent>(duration, isFriendly, damage);
  }
};

#endif
