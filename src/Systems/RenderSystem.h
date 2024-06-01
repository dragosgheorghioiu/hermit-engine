#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <memory>

class RenderSystem : public System {
public:
  RenderSystem();
  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore,
              SDL_Rect &camera);
};

#endif
