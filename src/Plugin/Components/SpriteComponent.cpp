#include "SpriteComponent.h"
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params) {
  int width = std::get<int>(component_params[0]);
  int height = std::get<int>(component_params[1]);
  const char *id = std::get<const char *>(component_params[2]);
  unsigned int zIndex = std::get<int>(component_params[3]);
  bool isFixed = std::get<bool>(component_params[4]);
  int srcRectX = std::get<int>(component_params[5]);
  int srcRectY = std::get<int>(component_params[6]);

  PluginSpriteComponent *sprite = new PluginSpriteComponent(
      width, height, id, zIndex, isFixed, srcRectX, srcRectY);
  return static_cast<void *>(sprite);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginSpriteComponent *>(instance);
}
extern "C" const char *getComponentName() { return "SpriteComponent"; }
