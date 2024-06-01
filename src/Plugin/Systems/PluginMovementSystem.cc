#include "PluginMovementSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"
#include <cstdlib>
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

      rigidBodyComponent->velocity.y +=
          rigidBodyComponent->acceleration.y / 100;
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
    onPlayerWallCollision(entity2, entity1);
  } else if (entity1.hasTag("player") && entity2.belongsGroup("walls")) {
    onPlayerWallCollision(entity1, entity2);
  }
}

void PluginMovementSystem::onPlayerWallCollision(EntityType &player,
                                                 EntityType &wall) {
  auto *rigidBody = static_cast<RigidBodyComponent *>(
      player.getComponent("RigidBodyComponent").instance);
  auto *transform = static_cast<PluginTransformComponent *>(
      player.getComponent("TransformComponent").instance);
  auto *playerBoxCollider = static_cast<BoxColliderComponent *>(
      player.getComponent("BoxColliderComponent").instance);
  auto *wallTransform = static_cast<PluginTransformComponent *>(
      wall.getComponent("TransformComponent").instance);
  auto *wallBoxCollider = static_cast<BoxColliderComponent *>(
      wall.getComponent("BoxColliderComponent").instance);

  // check if coming from the left
  if (transform->position.x +
              (playerBoxCollider->offset.x + playerBoxCollider->dimensions.x) *
                  transform->scale.x >
          wallTransform->position.x +
              wallBoxCollider->offset.x * wallTransform->scale.x &&
      transform->position.x + playerBoxCollider->offset.x * transform->scale.x <
          wallTransform->position.x +
              wallBoxCollider->offset.x * wallTransform->scale.x) {
    transform->position.x =
        wallTransform->position.x -
        (playerBoxCollider->offset.x + playerBoxCollider->dimensions.x) *
            transform->scale.x -
        1;
    rigidBody->velocity.x = 0;
    return;
  } else if (transform->position.x +
                     playerBoxCollider->offset.x * transform->scale.x <
                 wallTransform->position.x + (wallBoxCollider->offset.x +
                                              wallBoxCollider->dimensions.x) *
                                                 wallTransform->scale.x &&
             transform->position.x + (playerBoxCollider->offset.x +
                                      playerBoxCollider->dimensions.x) *
                                         transform->scale.x >
                 wallTransform->position.x + (wallBoxCollider->offset.x +
                                              wallBoxCollider->dimensions.x) *
                                                 wallTransform->scale.x) {
    transform->position.x =
        wallTransform->position.x +
        (wallBoxCollider->offset.x + wallBoxCollider->dimensions.x) *
            wallTransform->scale.x +
        1 - playerBoxCollider->offset.x * transform->scale.x;
    ;
    rigidBody->velocity.x = 0;
    return;
  } else

    // check if coming from the top
    if (transform->position.y + (playerBoxCollider->offset.y +
                                 playerBoxCollider->dimensions.y) *
                                    transform->scale.y >
            wallTransform->position.y +
                wallBoxCollider->offset.y * wallTransform->scale.y &&
        transform->position.y +
                playerBoxCollider->offset.y * transform->scale.y <
            wallTransform->position.y +
                wallBoxCollider->offset.y * wallTransform->scale.y) {
      transform->position.y =
          wallTransform->position.y -
          (playerBoxCollider->offset.y + playerBoxCollider->dimensions.y) *
              transform->scale.y -
          1;
      rigidBody->velocity.y = 0;
      rigidBody->isGrounded = true;
      return;
    } else {
      transform->position.y =
          wallTransform->position.y +
          (wallBoxCollider->offset.y + wallBoxCollider->dimensions.y) *
              wallTransform->scale.y +
          1 - playerBoxCollider->offset.y * transform->scale.y;
      ;
      rigidBody->velocity.y = 0;
      return;
    }
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
PluginMovementSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
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
