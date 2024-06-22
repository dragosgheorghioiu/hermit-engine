#include "SceneLoader.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"
#include "toml/get.hpp"
#include "toml/parser.hpp"
#include <filesystem>
#include <glm/ext/vector_float2.hpp>

SceneLoader::SceneLoader(std::filesystem::path scene_dir)
    : scene_dir(scene_dir){};

void SceneLoader::LoadEntities(const toml::value &toml_scene,
                               std::unique_ptr<RegistryType> &pluginRegistry,
                               std::unique_ptr<PluginLoader> &pluginLoader,
                               std::unique_ptr<AssetStore> &assetStore) {
  const auto entities = toml::find(toml_scene, "entities");
  for (const auto &entity : entities.as_array()) {
    const std::string tag = toml::find_or<std::string>(entity, "tag", "");
    const std::string group = toml::find_or<std::string>(entity, "group", "");

    auto new_entity = pluginRegistry->createEntity();
    Logger::Log("Entity created with id: " +
                std::to_string(new_entity.getId()));
    if (tag != "")
      new_entity.tag(tag);
    if (group != "")
      new_entity.group(group);

    if (tag == "" && group == "")
      new_entity.group("default");

    const auto components = toml::find(entity, "components");
    for (const auto &component : components.as_array()) {
      const std::string component_name =
          toml::find<std::string>(component, "type");
      const auto params = toml::find(component, "params").as_array();
      ComponentFactoryInfo componentInfo =
          pluginLoader->getComponentInfo(component_name);

      std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                               std::vector<bool>, std::vector<float>>>
          component_params;
      for (const auto &param : params) {
        const std::string type = toml::find<std::string>(param, "type");
        if (type == "int") {
          component_params.push_back(toml::find<int>(param, "value"));
        } else if (type == "float") {
          component_params.push_back(toml::find<float>(param, "value"));
        } else if (type == "string") {
          component_params.push_back(
              toml::find<std::string>(param, "value").c_str());
        } else if (type == "bool") {
          component_params.push_back(toml::find<bool>(param, "value"));
        } else if (type == "int_array") {
          const auto values = toml::find(param, "value").as_array();
          std::vector<int> int_values;
          for (const auto &value : values) {
            int_values.push_back(value.as_integer());
          }
          component_params.push_back(int_values);
        } else if (type == "float_array") {
          const auto values = toml::find(param, "value").as_array();
          std::vector<float> float_values;
          for (const auto &value : values) {
            float_values.push_back(value.as_floating());
          }
          component_params.push_back(float_values);
        } else if (type == "bool_array") {
          const auto values = toml::find(param, "value").as_array();
          std::vector<bool> bool_values;
          for (const auto &value : values) {
            bool_values.push_back(value.as_boolean());
          }
          component_params.push_back(bool_values);
        }
      }

      pluginRegistry->addComponentToEntity(new_entity, componentInfo,
                                           component_params);
    }
  }
}

SceneLoader::~SceneLoader() { Logger::Log("SceneLoader destroyed"); }

void SceneLoader::LoadScene(std::string level_path,
                            std::unique_ptr<RegistryType> &pluginRegistry,
                            std::unique_ptr<PluginLoader> &pluginLoader,
                            std::unique_ptr<AssetStore> &assetStore,
                            SDL_Renderer *renderer) {
  namespace fs = std::filesystem;
  fs::path scene_file;
  toml::basic_value<toml::discard_comments, std::unordered_map, std::vector>
      toml_scene;
  try {
    scene_file = scene_dir / level_path;
    toml_scene = toml::parse(scene_file);
  } catch (std::exception &e) {
    Logger::Err("No scene file found at: " + scene_file.string());
    throw e;
    return;
  }

  LoadAssets(toml_scene, assetStore, renderer);
  LoadTileMap(toml_scene, pluginRegistry, assetStore, pluginLoader, renderer);
  LoadEntities(toml_scene, pluginRegistry, pluginLoader, assetStore);
}

void SceneLoader::LoadAssets(const toml::value &toml_scene,
                             std::unique_ptr<AssetStore> &assetStore,
                             SDL_Renderer *renderer) {
  // clear AssetStore
  assetStore->Clear();

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
                              std::unique_ptr<RegistryType> &pluginRegistry,
                              std::unique_ptr<AssetStore> &assetStore,
                              std::unique_ptr<PluginLoader> &pluginLoader,
                              SDL_Renderer *renderer) {
  const auto tilemap = toml::find(toml_scene, "tilemap");
  std::filesystem::path map;
  try {
    map = std::filesystem::canonical(
        std::filesystem::current_path().parent_path().append(
            toml::find<std::string>(tilemap, "map")));
  } catch (std::exception &e) {
    Logger::Err("ERROR LOADING TILEMAP");
  }
  // check tilemap specifications
  const std::string mapTextureId = toml::find<std::string>(tilemap, "texture");
  const int tileScale = toml::find<int>(tilemap, "scale");
  const int tileSize = toml::find<int>(tilemap, "tileSize");
  const int mapColumns = toml::find<int>(tilemap, "cols");
  const int mapRows = toml::find<int>(tilemap, "rows");

  std::fstream mapFile;
  try {
    mapFile.open(map);
  } catch (std::exception &e) {
    Logger::Err("Could not open map file at: " + map.string());
    exit(1);
  }

  for (int row = 0; row < mapRows; row++) {
    for (int column = 0; column < mapColumns; column++) {
      char ch;
      mapFile.get(ch);
      int sourceRectY = atoi(&ch) * tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * tileSize;
      mapFile.ignore();

      EntityType tile = pluginRegistry->createEntity();
      tile.group("tile");
      std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                               std::vector<bool>, std::vector<float>>>
          component_params;

      ComponentFactoryInfo transformComponent =
          pluginLoader->getComponentInfo("TransformComponent");
      component_params.insert(component_params.begin(),
                              {column * tileSize * tileScale,
                               row * tileSize * tileScale, tileScale, tileScale,
                               0.0f});
      tile.addComponent(transformComponent, component_params);

      component_params.clear();

      ComponentFactoryInfo spriteComponent =
          pluginLoader->getComponentInfo("SpriteComponent");
      component_params.insert(component_params.begin(),
                              {tileSize, tileSize, mapTextureId.c_str(), 0,
                               false, sourceRectX, sourceRectY});
      tile.addComponent(spriteComponent, component_params);
    }
  }
  mapFile.close();

  Game::mapWidth = mapColumns * tileSize * tileScale;
  Game::mapHeight = mapRows * tileSize * tileScale;
}
