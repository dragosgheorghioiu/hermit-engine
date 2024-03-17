#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <memory>

class RenderSystem : public System {
public:
  RenderSystem() {
    // require tranform component
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }
  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore,
              SDL_Rect &camera) {
    // update component position every frame

    std::vector<Entity> entities = GetSystemEntities();

    // filter out entities that are not visible
    entities.erase(
        std::remove_if(
            entities.begin(), entities.end(),
            [&camera](const Entity &entity) {
              TransformComponent &transform =
                  entity.GetComponent<TransformComponent>();
              SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();
              if (sprite.isFixed)
                return false;
              if (transform.position.x + sprite.width * transform.scale.x <
                      camera.x ||
                  transform.position.y + sprite.height * transform.scale.y <
                      camera.y ||
                  transform.position.x > camera.x + camera.w ||
                  transform.position.y > camera.y + camera.h) {
                return true;
              }
              return false;
            }),
        entities.end());

    std::sort(entities.begin(), entities.end(),
              [](const Entity &a, const Entity &b) {
                return a.GetComponent<SpriteComponent>().zIndex <
                       b.GetComponent<SpriteComponent>().zIndex;
              });

    for (auto entity : entities) {
      // update entity position based on its velocity
      TransformComponent &transform = entity.GetComponent<TransformComponent>();
      SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();

      SDL_Rect srcRect = sprite.srcRect;
      SDL_Rect dstRect = {
          static_cast<int>(transform.position.x - !sprite.isFixed * camera.x),
          static_cast<int>(transform.position.y - !sprite.isFixed * camera.y),
          static_cast<int>(sprite.width * transform.scale.x),
          static_cast<int>(sprite.height * transform.scale.y)};

      SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.id), &srcRect,
                       &dstRect, transform.rotation, nullptr, sprite.flip);
    }
  }
};

#endif
