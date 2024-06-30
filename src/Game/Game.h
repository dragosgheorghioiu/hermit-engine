#ifndef GAME_H
#define GAME_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/Entity.h"
#include "../ECS/Registry.h"
#include "../Plugin/PluginLoader.h"
#include "SceneLoader/SceneLoader.h"
#include "imgui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <sol/sol.hpp>

const int FPS = 60;
const int MILLIS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool sceneExists;
  bool isRunning;
  bool isDebug;

  int milisecondsPrevFrame;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Rect camera;

  ImGuiIO imguiIO;

  static std::unique_ptr<RegistryType> pluginRegistry;
  static std::unique_ptr<AssetStore> assetStore;
  static std::unique_ptr<SceneLoader> sceneLoader;
  static std::unique_ptr<PluginLoader> pluginLoader;

  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
      allGuiElements;
  std::vector<float> memorySamples;
  std::vector<float> cpuSamples;
  sol::state lua;
  std::filesystem::path project_dir;
  std::filesystem::path config_dir;
  std::filesystem::path scene_dir;
  std::string current_scene;
  std::filesystem::path script_dir;
  std::string current_script;
  toml::basic_value<toml::discard_comments, std::unordered_map, std::vector>
      config_file;
  toml::value parsed_scene_path;

  int current_entity_id = -1;
  std::string current_entity_name = "";
  int current_component_id = -1;
  std::string current_component_name = "";
  std::string current_group = "";

public:
  Game();
  ~Game();
  void Init();
  void Run();
  void Destroy();

private:
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void GetConfig();

  void setComponentSignatureOfSystem(std::string systemName);
  void loadSystem(const std::pair<std::string, std::string> &systemNamePath);

  void addGUIElement(std::string systemName);
  void removeGUIElement(std::string systemName);

  void showMouseCursorPositionPanel();
  void showPropertyEditor();
  void showSystemLoaderPanel();
  void showSceneLoaderPanel();
  void showFPSCounter();
  void showMemoryUsage();
  void showCPUUsage();
  size_t GetCurrentMemoryUsage();
  float GetCurrentCPUUsage();

  void setLuaMappings();
  std::any solObjectToStdAny(const sol::object &obj);
  void createLuaTableForKeys();

  void clearSceneAndLoadScene(const std::string &sceneName);

  void writeToConfigFile();

public:
  static int windowWidth;
  static int windowHeight;
  static int mapWidth;
  static int mapHeight;
};

#endif
