#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <vector>

struct AnimationComponent {
  int animationIndex;
  std::vector<int> numFrames;
  int indexCurrentFrame;
  std::vector<int> frameSpeed;
  std::vector<bool> isLooping;
  int startTime;
  int originX;
  int originY;
  int spaceBetweenFramesX;
  int spaceBetweenFramesY;

  AnimationComponent(std::vector<int> numFrames, std::vector<int> frameSpeed,
                     std::vector<bool> isLooping, int originX, int originY,
                     int spaceBetweenFramesX, int spaceBetweenFramesY)
      : numFrames(numFrames), frameSpeed(frameSpeed), isLooping(isLooping),
        originX(originX), originY(originY),
        spaceBetweenFramesX(spaceBetweenFramesX),
        spaceBetweenFramesY(spaceBetweenFramesY) {
    animationIndex = 0;
    indexCurrentFrame = 0;
    startTime = SDL_GetTicks();
  }
};

#endif
