#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/Registry.h"
#include "Plugin/PluginLoader.h"
#include "toml/value.hpp"
#include <SDL2/SDL_render.h>
#include <filesystem>
#include <glm/ext/vector_float2.hpp>
#include <memory>

class SceneLoader {
private:
  static std::filesystem::path scene_dir;

  static void LoadAssets(const toml::value &toml_scene,
                         std::unique_ptr<AssetStore> &assetStore,
                         SDL_Renderer *renderer);
  static void LoadTileMap(const toml::value &toml_scene,
                          std::unique_ptr<RegistryType> &pluginRegistry,
                          std::unique_ptr<AssetStore> &assetStore,
                          SDL_Renderer *renderer);
  static void LoadEntities(const toml::value &toml_scene,
                           std::unique_ptr<RegistryType> &pluginRegistry,
                           std::unique_ptr<PluginLoader> &pluginLoader,
                           std::unique_ptr<AssetStore> &assetStore);

public:
  SceneLoader();
  ~SceneLoader();

  // static void ReadAllScenes();
  // static void LoadImGuiConfig();
  static void LoadScene(std::string scene,
                        std::unique_ptr<RegistryType> &pluginRegistry,
                        std::unique_ptr<PluginLoader> &pluginLoader,
                        std::unique_ptr<AssetStore> &assetStore,
                        SDL_Renderer *renderer);
};

#endif
