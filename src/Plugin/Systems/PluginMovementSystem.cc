#include "PluginMovementSystem.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include <iostream>

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

    if (rigidBodyComponent->velocity.x != 0 ||
        rigidBodyComponent->velocity.y != 0) {
      transformComponent->position.x +=
          rigidBodyComponent->velocity.x * deltaTime;
      transformComponent->position.y +=
          rigidBodyComponent->velocity.y * deltaTime;
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
  Logger::Log("Collision detected");
  // This is where the plugin's collision logic will go
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
