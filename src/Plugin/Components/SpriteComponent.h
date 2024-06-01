#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SDL2/SDL.h>
#include <string>
#include <sys/types.h>

struct PluginSpriteComponent {
  int width;
  int height;
  unsigned int zIndex;
  SDL_Rect srcRect;
  std::string id;
  bool isFixed;
  SDL_RendererFlip flip;

  PluginSpriteComponent(int width, int height, const std::string &id,
                        unsigned int zIndex = 0, bool isFixed = false,
                        int srcRectX = 0, int srcRectY = 0,
                        SDL_RendererFlip flip = SDL_FLIP_NONE)
      : width(width), height(height), zIndex(zIndex), id(id), isFixed(isFixed),
        flip(flip) {
    this->srcRect = {srcRectX, srcRectY, width, height};
  }
};

#endif
