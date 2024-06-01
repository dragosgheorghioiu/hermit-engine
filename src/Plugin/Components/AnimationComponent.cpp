#include "./AnimationComponent.h"
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params) {
  // cast the variant to the correct type
  std::vector<int> numFrames = std::get<std::vector<int>>(component_params[0]);
  std::vector<int> frameSpeed = std::get<std::vector<int>>(component_params[1]);
  std::vector<bool> isLooping =
      std::get<std::vector<bool>>(component_params[2]);
  int originX = std::get<int>(component_params[3]);
  int originY = std::get<int>(component_params[4]);
  int spaceBetweenFramesX = std::get<int>(component_params[5]);
  int spaceBetweenFramesY = std::get<int>(component_params[6]);

  // create the instance
  AnimationComponent *animation =
      new AnimationComponent(numFrames, frameSpeed, isLooping, originX, originY,
                             spaceBetweenFramesX, spaceBetweenFramesY);

  // return the instance
  return static_cast<void *>(animation);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<AnimationComponent *>(instance);
}
extern "C" const char *getComponentName() { return "AnimationComponent"; }
