#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SDL2/SDL.h>
#include <string>

struct SpriteComponent {
  int width;
  int height;
  SDL_Rect srcRect;
  std::string id;

  SpriteComponent(int width = 0, int height = 0, std::string id = "",
                  int srcRectX = 0, int srcRectY = 0)
      : width(width), height(height), id(id) {
    srcRect.x = srcRectX;
    srcRect.y = srcRectY;
    srcRect.w = width;
    srcRect.h = height;
  }
};

#endif
