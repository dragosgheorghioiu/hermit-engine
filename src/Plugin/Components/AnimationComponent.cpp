#include "./AnimationComponent.h"
#include "../PluginComponentFactory.h"
#include <sol/state.hpp>
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

extern "C" void addLuaBindings(sol::state &lua) {
  // animationIndex
  lua.set_function("get_animation_index", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return animationComponent->animationIndex + 1;
  });

  lua.set_function("set_animation_index",
                   [](ComponentInfo *component, int animationIndex) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->animationIndex = animationIndex - 1;
                   });

  // numFrames
  lua.set_function("get_animation_num_frames", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return sol::as_table(animationComponent->numFrames);
  });
  lua.set_function("set_animation_num_frames",
                   [](ComponentInfo *component, sol::table numFrames) {
                     std::vector<int> numFramesVector;
                     for (int i = 1; i <= numFrames.size(); i++) {
                       numFramesVector.push_back(numFrames[i]);
                     }
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->numFrames = numFramesVector;
                   });
  lua.set_function("set_animation_num_frames_at_index",
                   [](ComponentInfo *component, int index, int value) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     if (index <= 0) {
                       std::cerr << "Index must be greater than 0" << std::endl;
                       return;
                     }
                     animationComponent->numFrames[index - 1] = value;
                   });

  // frameSpeed
  lua.set_function("get_animation_speed", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return sol::as_table(animationComponent->frameSpeed);
  });
  lua.set_function("set_animation_speeds",
                   [](ComponentInfo *component, sol::table frameSpeed) {
                     std::vector<int> frameSpeedVector;
                     for (int i = 1; i <= frameSpeed.size(); i++) {
                       frameSpeedVector.push_back(frameSpeed[i]);
                     }
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->frameSpeed = frameSpeedVector;
                   });
  lua.set_function("set_animation_speed_at_index",
                   [](ComponentInfo *component, int index, int value) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->frameSpeed[index - 1] = value;
                   });

  // isLooping
  lua.set_function("get_animation_is_looping", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return sol::as_table(animationComponent->isLooping);
  });
  lua.set_function("set_animation_is_looping",
                   [](ComponentInfo *component, sol::table isLooping) {
                     std::vector<bool> isLoopingVector;
                     for (int i = 1; i <= isLooping.size(); i++) {
                       isLoopingVector.push_back(isLooping[i]);
                     }
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->isLooping = isLoopingVector;
                   });
  lua.set_function("set_animation_is_looping_at_index",
                   [](ComponentInfo *component, int index, bool value) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->isLooping[index - 1] = value;
                   });

  // originX
  lua.set_function("get_animation_origin_x", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return animationComponent->originX;
  });
  lua.set_function("set_animation_origin_x",
                   [](ComponentInfo *component, int originX) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->originX = originX;
                   });

  // originY
  lua.set_function("get_animation_origin_y", [](ComponentInfo *component) {
    auto animationComponent =
        static_cast<AnimationComponent *>(component->instance);
    return animationComponent->originY;
  });
  lua.set_function("set_animation_origin_y",
                   [](ComponentInfo *component, int originY) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->originY = originY;
                   });

  // spaceBetweenFramesX
  lua.set_function("get_animation_space_between_frames_x",
                   [](ComponentInfo *component) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     return animationComponent->spaceBetweenFramesX;
                   });
  lua.set_function("set_animation_space_between_frames_x",
                   [](ComponentInfo *component, int spaceBetweenFramesX) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     animationComponent->spaceBetweenFramesX =
                         spaceBetweenFramesX;
                   });

  // spaceBetweenFramesY
  lua.set_function("get_animation_space_between_frames_y",
                   [](ComponentInfo *component) {
                     auto animationComponent =
                         static_cast<AnimationComponent *>(component->instance);
                     return animationComponent->spaceBetweenFramesY;
                   });
}
