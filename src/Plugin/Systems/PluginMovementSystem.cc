#include "PluginMovementSystem.h"
#include "../Components/AnimationComponent.h"
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

  (*lua)["on_collision"](&entity1, &entity2);

  // if (entity1.belongsGroup("walls") && entity2.hasTag("player")) {
  //   onPlayerWallCollision(entity2, entity1);
  // } else if (entity1.hasTag("player") && entity2.belongsGroup("walls")) {
  //   onPlayerWallCollision(entity1, entity2);
  // } else if (entity1.belongsGroup("shadow") && entity2.belongsGroup("walls"))
  // {
  //   onPlayerWallCollision(entity1, entity2);
  // } else if (entity1.belongsGroup("walls") && entity2.belongsGroup("shadow"))
  // {
  //   onPlayerWallCollision(entity2, entity1);
  // }
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

  glm::vec2 mtv = getMinimumTranslationVector(
      glm::vec2(playerLeft, playerTop), glm::vec2(playerRight, playerBottom),
      glm::vec2(wallLeft, wallTop), glm::vec2(wallRight, wallBottom));

  transform->position.x += mtv.x;
  transform->position.y += mtv.y;

  if (mtv.x != 0) {
    rigidBody->velocity.x = 0;
  }
  if (mtv.y != 0) {
    rigidBody->velocity.y = 0;
  }
  if (mtv.y < 0) {
    (*lua)["player"]["is_grounded"] = true;
  }
}

glm::vec2 PluginMovementSystem::getMinimumTranslationVector(
    glm::vec2 player_min, glm::vec2 player_max, glm::vec2 wall_min,
    glm::vec2 wall_max) {
  float overlapX = 0;
  float overlapY = 0;

  overlapX = std::min(player_max.x - wall_min.x, wall_max.x - player_min.x);
  overlapY = std::min(player_max.y - wall_min.y, wall_max.y - player_min.y);

  if (overlapX < overlapY) {
    if (player_min.x < wall_min.x) {
      overlapX = -overlapX;
    }
    return glm::vec2(overlapX, 0);
  }
  if (player_min.y < wall_min.y) {
    overlapY = -overlapY;
  }
  return glm::vec2(0, overlapY);
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

extern "C" const char **getSubscribedEvents() {
  static const char *events[] = {"collisionEvent", nullptr};
  return events;
}
