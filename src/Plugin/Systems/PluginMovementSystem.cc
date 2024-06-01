#include "PluginMovementSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"
#include <algorithm>
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
    glm::vec2 &velocity = rigidBodyComponent->velocity;
    velocity.x += rigidBodyComponent->acceleration.x * deltaTime;
    velocity.y += rigidBodyComponent->acceleration.y * deltaTime;

    // clamp the velocity
    velocity.x = std::clamp(velocity.x, -rigidBodyComponent->maxVelocity.x,
                            rigidBodyComponent->maxVelocity.x);
    velocity.y = std::clamp(velocity.y, -rigidBodyComponent->maxVelocity.y,
                            rigidBodyComponent->maxVelocity.y);

    Logger::Log("Velocity: " + std::to_string(velocity.x) + ", " +
                std::to_string(velocity.y));

    lua->script("print('Hello from Lua!')");

    transformComponent->position.x += velocity.x * deltaTime;
    transformComponent->position.y += velocity.y * deltaTime;
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

  // stop the player from going through the wall
  // get the corners of the player
  int playerTop =
      transform->position.y + playerBoxCollider->offset.y * transform->scale.y;
  int playerBottom =
      playerTop + playerBoxCollider->dimensions.y * transform->scale.y;
  int playerLeft =
      transform->position.x + playerBoxCollider->offset.x * transform->scale.x;
  int playerRight =
      playerLeft + playerBoxCollider->dimensions.x * transform->scale.x;

  // get the corners of the wall
  int wallTop = wallTransform->position.y +
                wallBoxCollider->offset.y * wallTransform->scale.y;
  int wallBottom =
      wallTop + wallBoxCollider->dimensions.y * wallTransform->scale.y;
  int wallLeft = wallTransform->position.x +
                 wallBoxCollider->offset.x * wallTransform->scale.x;
  int wallRight =
      wallLeft + wallBoxCollider->dimensions.x * wallTransform->scale.x;

  if (rigidBody->velocity.x > 0) {
    // Check for overlap on the right side
    if (playerRight > wallLeft && playerLeft < wallLeft &&
        playerBottom > wallTop && playerTop < wallBottom) {
      // Move the player to the left of the wall
      transform->position.x =
          wallLeft - playerBoxCollider->dimensions.x * transform->scale.x -
          playerBoxCollider->offset.x * transform->scale.x;
      rigidBody->velocity.x = 0;
    }
  }
  // Player is moving left
  else if (rigidBody->velocity.x < 0) {
    // Check for overlap on the left side
    if (playerLeft < wallRight && playerRight > wallRight &&
        playerBottom > wallTop && playerTop < wallBottom) {
      // Move the player to the right of the wall
      transform->position.x =
          wallRight - playerBoxCollider->offset.x * transform->scale.x;
      rigidBody->velocity.x = 0;
    }
  }
  // Player is moving up
  if (rigidBody->velocity.y > 0) {
    // Check for overlap on the top side
    if (playerBottom > wallTop && playerTop < wallTop &&
        playerRight > wallLeft && playerLeft < wallRight) {
      // Move the player to the top of the wall
      transform->position.y =
          wallTop - playerBoxCollider->dimensions.y * transform->scale.y -
          playerBoxCollider->offset.y * transform->scale.y;
      rigidBody->velocity.y = 0;
    }
  }
  // Player is moving down
  else if (rigidBody->velocity.y < 0) {
    // Check for overlap on the bottom side
    if (playerTop < wallBottom && playerBottom > wallBottom &&
        playerRight > wallLeft && playerLeft < wallRight) {
      // Move the player to the bottom of the wall
      transform->position.y =
          wallBottom - playerBoxCollider->offset.y * transform->scale.y;
      rigidBody->velocity.y = 0;
    }
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
