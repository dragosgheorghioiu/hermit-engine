#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

struct AnimationComponent {
  size_t numFrames;
  int indexCurrentFrame;
  int frameSpeed;
  bool isLooping;
  int startTime;

  AnimationComponent(size_t numFrames = 1, int frameSpeed = 1,
                     bool isLooping = true)
      : numFrames(numFrames), indexCurrentFrame(1), frameSpeed(frameSpeed),
        isLooping(isLooping), startTime(SDL_GetTicks()) {}
};

#endif
