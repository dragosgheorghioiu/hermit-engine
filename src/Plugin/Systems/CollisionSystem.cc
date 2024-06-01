#include "CollisionSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../PluginLoader.h"

CollisionSystem::CollisionSystem() = default;
CollisionSystem::~CollisionSystem() = default;

void CollisionSystem::update(std::vector<void *> params) {
  PluginLoader *loader = static_cast<PluginLoader *>(params[0]);
  std::vector<EntityType> entities = getSystemEntities();

  for (auto it = entities.begin(); it != entities.end(); it++) {
    BoxColliderComponent *boxCollider = static_cast<BoxColliderComponent *>(
        it->getComponent("BoxColliderComponent").instance);
    PluginTransformComponent *transform =
        static_cast<PluginTransformComponent *>(
            it->getComponent("TransformComponent").instance);

    int currentEntityTop =
        transform->position.y + boxCollider->offset.y * transform->scale.y;
    int currentEntityBottom =
        currentEntityTop + boxCollider->dimensions.y * transform->scale.y;
    int currentEntityLeft =
        transform->position.x + boxCollider->offset.x * transform->scale.x;
    int currentEntityRight =
        currentEntityLeft + boxCollider->dimensions.x * transform->scale.x;

    for (auto it2 = it + 1; it2 != entities.end(); it2++) {
      BoxColliderComponent *boxCollider2 = static_cast<BoxColliderComponent *>(
          it2->getComponent("BoxColliderComponent").instance);
      PluginTransformComponent *transform2 =
          static_cast<PluginTransformComponent *>(
              it2->getComponent("TransformComponent").instance);
      int otherEntityTop =
          transform2->position.y + boxCollider2->offset.y * transform2->scale.y;
      int otherEntityBottom =
          otherEntityTop + boxCollider2->dimensions.y * transform2->scale.y;
      int otherEntityLeft =
          transform2->position.x + boxCollider2->offset.x * transform2->scale.x;
      int otherEntityRight =
          otherEntityLeft + boxCollider2->dimensions.x * transform2->scale.x;

      if (currentEntityRight >= otherEntityLeft &&
          currentEntityLeft <= otherEntityRight &&
          currentEntityBottom >= otherEntityTop &&
          currentEntityTop <= otherEntityBottom) {
        loader->getEventFactory().triggerEvent("collisionEvent", *it, *it2);
      }
    }
  }
}

const std::string CollisionSystem::GetName() const { return "CollisionSystem"; }

std::function<void(void *)>
CollisionSystem::getCallback(std::string eventType) {
  return nullptr;
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
CollisionSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
}

extern "C" void *createInstance() { return new CollisionSystem(); }

extern "C" void destroyInstance(void *instance) {
  delete (CollisionSystem *)instance;
}

extern "C" const char *getName() { return "CollisionSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"BoxColliderComponent",
                                     "TransformComponent", nullptr};
  return components;
}
