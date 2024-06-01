#include "CollisionSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"

CollisionSystem::CollisionSystem() {
  RequireComponent<BoxColliderComponent>();
  RequireComponent<TransformComponent>();
}

void CollisionSystem::Update(std::unique_ptr<EventBus> &eventBus) {
  auto entities = GetSystemEntities();
  for (auto it = entities.begin(); it != entities.end(); it++) {
    Entity &entity = *it;
    BoxColliderComponent &currentEnityBoxCollider =
        entity.GetComponent<BoxColliderComponent>();
    TransformComponent &currentEntityTransform =
        entity.GetComponent<TransformComponent>();

    int currentEntityTop =
        currentEntityTransform.position.y + currentEnityBoxCollider.offset.y;
    int currentEntityBottom =
        currentEntityTop + currentEnityBoxCollider.dimensions.y *
                               static_cast<int>(currentEntityTransform.scale.y);
    int currentEntityLeft =
        currentEntityTransform.position.x + currentEnityBoxCollider.offset.x;
    int currentEntityRight =
        currentEntityLeft +
        currentEnityBoxCollider.dimensions.x *
            static_cast<int>(currentEntityTransform.scale.x);

    for (auto otherIt = it + 1; otherIt != entities.end(); otherIt++) {
      Entity &otherEntity = *otherIt;

      BoxColliderComponent &otherEntityBoxCollider =
          otherEntity.GetComponent<BoxColliderComponent>();
      TransformComponent &otherEntityTransform =
          otherEntity.GetComponent<TransformComponent>();

      // AABB collision detection
      int otherEntityTop =
          otherEntityTransform.position.y + otherEntityBoxCollider.offset.y;
      int otherEntityBottom =
          otherEntityTop + otherEntityBoxCollider.dimensions.y *
                               static_cast<int>(otherEntityTransform.scale.y);
      int otherEntityLeft =
          otherEntityTransform.position.x + otherEntityBoxCollider.offset.x;
      int otherEntityRight =
          otherEntityLeft + otherEntityBoxCollider.dimensions.x *
                                static_cast<int>(otherEntityTransform.scale.x);

      if (currentEntityRight >= otherEntityLeft &&
          currentEntityLeft <= otherEntityRight &&
          currentEntityBottom >= otherEntityTop &&
          currentEntityTop <= otherEntityBottom) {
        eventBus->EmitEvent<CollisionEvent>(entity, otherEntity);
      }
    }
  }
}
