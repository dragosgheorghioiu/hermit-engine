#include "PluginMovementSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"
#include <cstdlib>
#include <iostream>
#include <string>

PluginMovementSystem::PluginMovementSystem() = default;
PluginMovementSystem::~PluginMovementSystem() = default;

void PluginMovementSystem::update(std::vector<void *> params) {
  double deltaTime = *static_cast<double *>(params[0]);

  std::vector<EntityType> entities = getSystemEntities();
  for (auto entity : entities) {
    RigidBodyComponent *rigidBodyComponent = static_cast<RigidBodyComponent *>(
        entity.getComponent("RigidBodyComponent").instance);
    PluginTransformComponent *transformComponent =
        static_cast<PluginTransformComponent *>(
            entity.getComponent("TransformComponent").instance);

    // prevent player from moving off screen
    if (entity.hasTag("player")) {
      auto *currentEnityBoxCollider = static_cast<BoxColliderComponent *>(
          entity.getComponent("BoxColliderComponent").instance);
      if (transformComponent->position.x +
                  transformComponent->scale.x *
                      currentEnityBoxCollider->offset.x <
              0 ||
          transformComponent->position.x +
                  transformComponent->scale.x *
                      (currentEnityBoxCollider->offset.x +
                       currentEnityBoxCollider->dimensions.x) >
              1920) {
        rigidBodyComponent->velocity.x = 0;
      }
      if (transformComponent->position.y +
                  transformComponent->scale.y *
                      currentEnityBoxCollider->offset.y <
              0 ||
          transformComponent->position.y +
                  transformComponent->scale.y *
                      (currentEnityBoxCollider->offset.y +
                       currentEnityBoxCollider->dimensions.y) >
              1080) {
        rigidBodyComponent->velocity.y = 0;
      }
    }

    if (rigidBodyComponent->velocity.x != 0 ||
        rigidBodyComponent->velocity.y != 0) {
      transformComponent->position.x +=
          rigidBodyComponent->velocity.x * deltaTime;
      transformComponent->position.y +=
          rigidBodyComponent->velocity.y * deltaTime;
      if (std::abs(rigidBodyComponent->maxVelocity.x) >
          std::abs(rigidBodyComponent->velocity.x))
        rigidBodyComponent->velocity.x +=
            rigidBodyComponent->acceleration.x / 100 *
            (rigidBodyComponent->velocity.x < 0
                 ? -1
                 : 1); // * -1 if negative velocity
      else
        rigidBodyComponent->velocity.x =
            rigidBodyComponent->maxVelocity.x *
            (rigidBodyComponent->velocity.x < 0 ? -1 : 1);

      if (std::abs(rigidBodyComponent->maxVelocity.y) >
          std::abs(rigidBodyComponent->velocity.y))
        rigidBodyComponent->velocity.y +=
            rigidBodyComponent->acceleration.y / 100 *
            (rigidBodyComponent->velocity.y < 0 ? -1 : 1);
      else
        rigidBodyComponent->velocity.y =
            rigidBodyComponent->maxVelocity.y *
            (rigidBodyComponent->velocity.y < 0 ? -1 : 1);

      std::cout << "Velocity: " << rigidBodyComponent->velocity.x << ", "
                << rigidBodyComponent->velocity.y << std::endl;
    }
  }
}

const std::string PluginMovementSystem::GetName() const {
  return "PluginMovementSystem";
}

std::function<void(void *)>
PluginMovementSystem::getCallback(std::string eventType) {
  if (eventType == "collisionEvent") {
    return [this](void *event) { this->onCollision(event); };
  }
  return nullptr;
}

void PluginMovementSystem::onCollision(void *event) {
  auto *collisionEvent = static_cast<CollisionEvent *>(event);
  auto entity1 = collisionEvent->entity1;
  auto entity2 = collisionEvent->entity2;

  if (entity1.belongsGroup("walls") && entity2.hasTag("player")) {
    Logger::Debug("Player wall collision");
    onPlayerWallCollision(entity2, entity1);
  } else if (entity1.hasTag("player") && entity2.belongsGroup("walls")) {
    Logger::Debug("Player wall collision");
    onPlayerWallCollision(entity1, entity2);
  }
}

void PluginMovementSystem::onPlayerWallCollision(EntityType &player,
                                                 EntityType &wall) {
  auto *rigidBody = static_cast<RigidBodyComponent *>(
      player.getComponent("RigidBodyComponent").instance);
  auto *transform = static_cast<PluginTransformComponent *>(
      player.getComponent("TransformComponent").instance);

  // reverse player velocity
  rigidBody->velocity.x = -rigidBody->velocity.x;
  rigidBody->velocity.y = -rigidBody->velocity.y;

  // flip player sprite
  auto *sprite = static_cast<PluginSpriteComponent *>(
      player.getComponent("SpriteComponent").instance);
  sprite->flip =
      sprite->flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

std::unordered_map<std::string, std::function<void()>>
PluginMovementSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void()>>();
}

extern "C" void *createInstance() { return new PluginMovementSystem(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginMovementSystem *>(instance);
}
extern "C" const char *getName() { return "PluginMovementSystem"; }
extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"RigidBodyComponent", "TransformComponent",
                                     nullptr};
  return components;
}
