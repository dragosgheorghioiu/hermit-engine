#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../../toml/include/toml.hpp"
#include "../AssetStore/AssetStore.h"
#include "../ECS/Registry.h"
#include "Plugin/PluginLoader.h"
#include "include/toml11/types.hpp"
#include <SDL2/SDL_render.h>
#include <filesystem>
#include <glm/ext/vector_float2.hpp>
#include <memory>

class SceneLoader {
private:
  std::filesystem::path scene_dir;
  toml::value toml_entities;

  void LoadAssets(const toml::value &toml_scene,
                  std::unique_ptr<AssetStore> &assetStore,
                  SDL_Renderer *renderer);
  void LoadTileMap(const toml::value &toml_scene,
                   std::unique_ptr<RegistryType> &pluginRegistry,
                   std::unique_ptr<AssetStore> &assetStore,
                   std::unique_ptr<PluginLoader> &pluginLoader,
                   SDL_Renderer *renderer);
  void LoadEntities(const toml::value &toml_scene,
                    std::unique_ptr<RegistryType> &pluginRegistry,
                    std::unique_ptr<PluginLoader> &pluginLoader,
                    std::unique_ptr<AssetStore> &assetStore);

public:
  SceneLoader(std::filesystem::path scene_dir);
  ~SceneLoader();

  void LoadScene(std::string scene,
                 std::unique_ptr<RegistryType> &pluginRegistry,
                 std::unique_ptr<PluginLoader> &pluginLoader,
                 std::unique_ptr<AssetStore> &assetStore,
                 SDL_Renderer *renderer);
  toml::value &GetTomlEntities();
};

#endif
