#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class MovementSystem : public System {
public:
  MovementSystem() {
    // require tranform component
    RequireComponent<RigidBodyComponent>();
    RequireComponent<TransformComponent>();
  }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent(this, &MovementSystem::onCollision);
  }

  void onCollision(CollisionEvent &e) {
    Entity entity1 = e.entity1;
    Entity entity2 = e.entity2;

    if (entity1.BelongsGroup("obstacles") && entity2.BelongsGroup("enemy")) {
      OnEnemyObstacleCollision(entity2, entity1);
    } else if (entity1.BelongsGroup("enemy") &&
               entity2.BelongsGroup("obstacles")) {
      OnEnemyObstacleCollision(entity1, entity2);
    }
  }

  void OnEnemyObstacleCollision(Entity &enemy, Entity &obstacle) {
    if (enemy.HasComponent<RigidBodyComponent>()) {
      RigidBodyComponent &rigidBody = enemy.GetComponent<RigidBodyComponent>();

      // reverse enemy velocity
      rigidBody.velocity.x = -rigidBody.velocity.x;
      rigidBody.velocity.y = -rigidBody.velocity.y;
    }

    if (enemy.HasComponent<SpriteComponent>()) {
      SpriteComponent &sprite = enemy.GetComponent<SpriteComponent>();
      sprite.flip =
          sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }
  }

  void Update(double dt) {
    // update component position every frame
    //
    for (auto enity : GetSystemEntities()) {
      // update entity position based on its velocity
      TransformComponent &transform = enity.GetComponent<TransformComponent>();
      const RigidBodyComponent &rigidBody =
          enity.GetComponent<RigidBodyComponent>();

      transform.position.x += rigidBody.velocity.x * dt;
      transform.position.y += rigidBody.velocity.y * dt;

      // prevent player from going out of bounds
      // TODO: stop presuming the fact that the player has a sprite component
      if (enity.HasTag("player")) {
        SpriteComponent &sprite = enity.GetComponent<SpriteComponent>();
        transform.position.x = std::max(0.0f, transform.position.x);
        transform.position.y = std::max(0.0f, transform.position.y);
        transform.position.x =
            std::min(Game::mapWidth - sprite.width * transform.scale.x,
                     transform.position.x);
        transform.position.y =
            std::min(Game::mapHeight - sprite.height * transform.scale.y,
                     transform.position.y);
      }

      bool isOutOfBounds =
          transform.position.x < 0 || transform.position.x > Game::mapWidth ||
          transform.position.y < 0 || transform.position.y > Game::mapHeight;
      if (isOutOfBounds && !enity.HasTag("player")) {
        enity.Kill();
      }
    }
  }
};

#endif
