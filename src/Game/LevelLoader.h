#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "sol/sol.hpp"
#include <SDL2/SDL_render.h>
#include <memory>

class LevelLoader {
public:
  LevelLoader();
  ~LevelLoader();

  static void LoadLevel(sol::state &lua, int level,
                        std::unique_ptr<Registry> &registry,
                        std::unique_ptr<AssetStore> &assetStore,
                        SDL_Renderer *renderer);
};

#endif
