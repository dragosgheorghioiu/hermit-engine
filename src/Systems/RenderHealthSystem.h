#ifndef RENDER_HEALTH_SYSTEM_H
#define RENDER_HEALTH_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

class RenderHealthSystem : public System {
public:
  RenderHealthSystem();
  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore,
              SDL_Rect &camera);
};

#endif
