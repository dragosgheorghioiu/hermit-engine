#ifndef GAME_H
#define GAME_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

const int FPS = 60;
const int MILLIS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool isRunning;
  int milisecondsPrevFrame = 0;
  SDL_Window *window;
  SDL_Renderer *renderer;

  std::unique_ptr<Registry> registry;
  std::unique_ptr<AssetStore> assetStore;

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

  int windowWidth;
  int windowHeight;
};

#endif
