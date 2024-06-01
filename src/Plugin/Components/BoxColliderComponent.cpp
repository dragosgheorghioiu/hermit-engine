#include "BoxColliderComponent.h"
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params) {
  int x = std::get<int>(component_params[0]);
  int y = std::get<int>(component_params[1]);
  int width = std::get<int>(component_params[2]);
  int height = std::get<int>(component_params[3]);

  BoxColliderComponent *boxCollider =
      new BoxColliderComponent(glm::vec2(x, y), glm::vec2(width, height));

  return static_cast<void *>(boxCollider);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<BoxColliderComponent *>(instance);
}

extern "C" const char *getComponentName() { return "BoxColliderComponent"; }
