#ifndef RENDER_TEXT_LABEL_SYSTEM_H
#define RENDER_TEXT_LABEL_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL_render.h>

class RenderTextLabelSystem : public System {
public:
  RenderTextLabelSystem();

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore,
              SDL_Rect &camera);
};

#endif
