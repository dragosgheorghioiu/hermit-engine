#ifndef RENDERCOLLISIONSYSTEM_H
#define RENDERCOLLISIONSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

class RenderCollisionSystem : public System {
public:
  RenderCollisionSystem() {
    RequireComponent<BoxColliderComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update(SDL_Renderer *renderer, SDL_Rect &camera) {
    auto entities = GetSystemEntities();
    for (auto it = entities.begin(); it != entities.end(); it++) {
      Entity &entity = *it;
      BoxColliderComponent &currentEnityBoxCollider =
          entity.GetComponent<BoxColliderComponent>();
      TransformComponent &currentEntityTransform =
          entity.GetComponent<TransformComponent>();

      SDL_Rect collider = {
          static_cast<int>(currentEntityTransform.position.x +
                           currentEnityBoxCollider.offset.x - camera.x),
          static_cast<int>(currentEntityTransform.position.y +
                           currentEnityBoxCollider.offset.y - camera.y),
          static_cast<int>(currentEnityBoxCollider.dimensions.x *
                           currentEntityTransform.scale.x),
          static_cast<int>(currentEnityBoxCollider.dimensions.y *
                           currentEntityTransform.scale.y)};

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &collider);
    }
  }
};

#endif
