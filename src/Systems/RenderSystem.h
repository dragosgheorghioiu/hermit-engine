#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponents.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <memory>

class RenderSystem : public System {
public:
  RenderSystem() {
    // require tranform component
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }
  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {
    // update component position every frame
    //
    for (auto entity : GetSystemEntites()) {
      // update entity position based on its velocity
      TransformComponent &transform = entity.GetComponent<TransformComponent>();
      SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();

      SDL_Rect srcRect = sprite.srcRect;
      SDL_Rect dstRect = {static_cast<int>(transform.position.x),
                          static_cast<int>(transform.position.y),
                          static_cast<int>(sprite.width * transform.scale.x),
                          static_cast<int>(sprite.height * transform.scale.y)};

      SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.id), &srcRect,
                       &dstRect, transform.rotation, nullptr, SDL_FLIP_NONE);
    }
  }
};

#endif