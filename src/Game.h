#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

class Game {
private:
  bool isRunning;
  SDL_Window *window;
  SDL_Renderer *renderer;

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
