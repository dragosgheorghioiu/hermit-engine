#ifndef RENDERCOLLISIONSYSTEM_H
#define RENDERCOLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

class RenderCollisionSystem : public System {
public:
  RenderCollisionSystem();

  void Update(SDL_Renderer *renderer, SDL_Rect &camera);
};

#endif
