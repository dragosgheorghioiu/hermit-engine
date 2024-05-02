#include "SceneLoader.h"
// #include "../Components/AnimationComponent.h"
// #include "../Components/BoxColliderComponent.h"
// #include "../Components/CameraFollowComponent.h"
// #include "../Components/HealthComponent.h"
// #include "../Components/KeyboardControlComponent.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "toml/get.hpp"
#include "toml/parser.hpp"
#include <filesystem>

std::filesystem::path SceneLoader::scene_dir;

SceneLoader::SceneLoader() {
  std::string scenes_path;

  try {
    scenes_path = toml::find<std::string>(Game::config_file, "scenes_path");
  } catch (toml::type_error err) {
    Logger::Err("Could not find scenes_path");
    return;
  }

  scene_dir = std::filesystem::canonical(
      std::filesystem::current_path().parent_path().append(scenes_path));
  Logger::Log("SceneLoader created");
}

SceneLoader::~SceneLoader() { Logger::Log("SceneLoader destroyed"); }

void SceneLoader::LoadScene(std::string level_path,
                            std::unique_ptr<Registry> &registry,
                            std::unique_ptr<AssetStore> &assetStore,
                            SDL_Renderer *renderer) {
  namespace fs = std::filesystem;

  fs::path scene_file = scene_dir.append(level_path);
  const auto toml_scene = toml::parse(scene_file);

  LoadAssets(toml_scene, assetStore, renderer);
  LoadTileMap(toml_scene, registry, assetStore, renderer);
}

void SceneLoader::LoadAssets(const toml::value &toml_scene,
                             std::unique_ptr<AssetStore> &assetStore,
                             SDL_Renderer *renderer) {
  // load textures
  const auto textures = toml::find(toml_scene, "textures");
  for (const auto &texture : textures.as_array()) {
    const std::string id = toml::find<std::string>(texture, "id");
    const std::string file = toml::find<std::string>(texture, "file");
    assetStore->AddTexture(renderer, id, file);
  }

  // load fonts
  const auto fonts = toml::find(toml_scene, "fonts");
  for (const auto &font : fonts.as_array()) {
    const std::string id = toml::find<std::string>(font, "id");
    const std::string file = toml::find<std::string>(font, "file");
    const int size = toml::find<int>(font, "fontSize");
    assetStore->AddFont(id, file, size);
  }
}

void SceneLoader::LoadTileMap(const toml::value &toml_scene,
                              std::unique_ptr<Registry> &registry,
                              std::unique_ptr<AssetStore> &assetStore,
                              SDL_Renderer *renderer) {
  const auto tilemap = toml::find(toml_scene, "tilemap");
  const std::filesystem::path map = std::filesystem::canonical(
      std::filesystem::current_path().parent_path().append(
          toml::find<std::string>(tilemap, "map")));
  const std::string mapTextureId = toml::find<std::string>(tilemap, "texture");
  const int tileScale = toml::find<int>(tilemap, "scale");
  const int tileSize = toml::find<int>(tilemap, "tileSize");
  const int mapColumns = toml::find<int>(tilemap, "cols");
  const int mapRows = toml::find<int>(tilemap, "rows");

  std::fstream mapFile;
  mapFile.open(map);
  if (!mapFile.is_open()) {
    Logger::Err("ERROR LOADING TILEMAP");
    return;
  }

  for (int row = 0; row < mapRows; row++) {
    for (int column = 0; column < mapColumns; column++) {
      char ch;
      mapFile.get(ch);
      int sourceRectY = atoi(&ch) * tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = registry->CreateEntity();
      tile.Group("tile");
      tile.AddComponent<TransformComponent>(
          glm::vec2(column * tileSize * tileScale, row * tileSize * tileScale),
          glm::vec2(tileScale, tileScale));
      tile.AddComponent<SpriteComponent>(tileSize, tileSize, mapTextureId, 0,
                                         false, sourceRectX, sourceRectY);
    }
  }
  mapFile.close();

  Game::mapWidth = mapColumns * tileSize * tileScale;
  Game::mapHeight = mapRows * tileSize * tileScale;
}
