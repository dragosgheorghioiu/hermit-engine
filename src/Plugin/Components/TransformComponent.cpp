#include "TransformComponent.h"
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params) {

  int x = std::get<int>(component_params[0]);
  int y = std::get<int>(component_params[1]);
  int scaleX = std::get<int>(component_params[2]);
  int scaleY = std::get<int>(component_params[3]);
  float rotation = std::get<float>(component_params[4]);

  PluginTransformComponent *transform = new PluginTransformComponent(
      glm::vec2(x, y), glm::vec2(scaleX, scaleY), rotation);

  return static_cast<void *>(transform);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginTransformComponent *>(instance);
}
extern "C" const char *getComponentName() { return "TransformComponent"; }
