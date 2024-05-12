#include "./AnimationComponent.h"
#include <vector>

extern "C" void *createInstance(std::vector<int> numFrames,
                                std::vector<int> frameSpeed,
                                std::vector<bool> isLooping, int originX,
                                int originY, int spaceBetweenFramesX,
                                int spaceBetweenFramesY) {
  AnimationComponent *animation =
      new AnimationComponent(numFrames, frameSpeed, isLooping, originX, originY,
                             spaceBetweenFramesX, spaceBetweenFramesY);
  return static_cast<void *>(animation);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<AnimationComponent *>(instance);
}
extern "C" const char *getComponentName() { return "AnimationComponent"; }
