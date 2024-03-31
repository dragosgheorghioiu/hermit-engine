#include "SceneLoader.h"
// #include "../Components/AnimationComponent.h"
// #include "../Components/BoxColliderComponent.h"
// #include "../Components/CameraFollowComponent.h"
// #include "../Components/HealthComponent.h"
// #include "../Components/KeyboardControlComponent.h"
#include "../Logger/Logger.h"
#include "toml/exception.hpp"
#include "toml/get.hpp"
#include "toml/parser.hpp"
#include <filesystem>

std::filesystem::path SceneLoader::config_dir;
std::filesystem::path SceneLoader::scene_dir;

SceneLoader::SceneLoader() {
  config_dir = std::filesystem::current_path().parent_path().append("config");
  toml::basic_value<toml::discard_comments, std::unordered_map, std::vector>
      config_file;
  std::string scenes_path;

  try {
    config_file = toml::parse(config_dir.append("config.toml"));
  } catch (toml::syntax_error err) {
    Logger::Err("Could not parse config file");
    return;
  }

  try {
    scenes_path = toml::find<std::string>(config_file, "scenes_path");
  } catch (toml::type_error err) {
    Logger::Err("Could not find scenes_path");
    return;
  }

  scene_dir = std::filesystem::canonical(
      std::filesystem::current_path().parent_path().append(scenes_path));
  Logger::Log("SceneLoader created");
}

SceneLoader::~SceneLoader() { Logger::Log("SceneLoader destroyed"); }

void SceneLoader::LoadScene(std::string level,
                            std::unique_ptr<Registry> &registry,
                            std::unique_ptr<AssetStore> &assetStore,
                            SDL_Renderer *renderer) {
  toml::basic_value<toml::discard_comments, std::unordered_map, std::vector>
      scene_file;
  for (const std::filesystem::directory_entry &scene :
       std::filesystem::directory_iterator(scene_dir)) {
    if (scene.path().filename() == level)
      scene_file = toml::parse(scene.path());
  }

  std::string test_string = toml::find_or(scene_file, "name", "not_found");

  Logger::Warn(test_string);
}

void SceneLoader::ReadAllScenes() {
  std::filesystem::path config_dir =
      std::filesystem::current_path().parent_path().append("config");
}
