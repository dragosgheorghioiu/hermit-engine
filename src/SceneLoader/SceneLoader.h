#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "toml/value.hpp"
#include <SDL2/SDL_render.h>
#include <filesystem>
#include <memory>

class SceneLoader {
private:
  static std::filesystem::path config_dir;
  static std::filesystem::path scene_dir;

  static void LoadAssets(const toml::value &toml_scene,
                         std::unique_ptr<AssetStore> &assetStore,
                         SDL_Renderer *renderer);
  static void LoadTileMap(const toml::value &toml_scene,
                          std::unique_ptr<Registry> &registry,
                          std::unique_ptr<AssetStore> &assetStore,
                          SDL_Renderer *renderer);

public:
  SceneLoader();
  ~SceneLoader();

  static void ReadAllScenes();
  static void LoadImGuiConfig();
  static void LoadScene(std::string scene, std::unique_ptr<Registry> &registry,
                        std::unique_ptr<AssetStore> &assetStore,
                        SDL_Renderer *renderer);
};

#endif
