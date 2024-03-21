#ifndef GAME_H
#define GAME_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "imgui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <sol/sol.hpp>

const int FPS = 60;
const int MILLIS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool isRunning;
  bool isDebug;
  int milisecondsPrevFrame = 0;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Rect camera;
  ImGuiIO imguiIO;

  sol::state lua;

  std::unique_ptr<Registry> registry;
  std::unique_ptr<AssetStore> assetStore;
  std::unique_ptr<EventBus> eventBus;

public:
  Game();
  ~Game();
  void Init();
  void Run();
  void Destroy();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();

  static int windowWidth;
  static int windowHeight;
  static int mapWidth;
  static int mapHeight;
};

#endif
