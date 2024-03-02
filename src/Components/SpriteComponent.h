#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SDL2/SDL.h>
#include <string>
#include <sys/types.h>

struct SpriteComponent {
  int width;
  int height;
  unsigned int zIndex;
  SDL_Rect srcRect;
  std::string id;
  bool isFixed;

  SpriteComponent(int width = 0, int height = 0, std::string id = "",
                  unsigned int zIndex = 0, bool isFixed = false,
                  int srcRectX = 0, int srcRectY = 0)
      : width(width), height(height), zIndex(zIndex), id(id), isFixed(isFixed) {
    this->srcRect = {srcRectX, srcRectY, width, height};
  }
};

#endif
