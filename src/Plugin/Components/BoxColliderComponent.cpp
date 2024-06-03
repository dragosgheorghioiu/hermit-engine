#include "BoxColliderComponent.h"
#include "../PluginComponentFactory.h"
#include <sol/state.hpp>
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

extern "C" void addLuaBindings(sol::state &lua) {
  // position x
  lua.set_function("get_box_collider_position_x", [](ComponentInfo *component) {
    auto boxColliderComponent =
        static_cast<BoxColliderComponent *>(component->instance);
    return boxColliderComponent->offset.x;
  });
  lua.set_function(
      "set_box_collider_position_x", [](ComponentInfo *component, int x) {
        auto boxColliderComponent =
            static_cast<BoxColliderComponent *>(component->instance);
        boxColliderComponent->offset.x = x;
      });

  // position y
  lua.set_function("get_box_collider_position_y", [](ComponentInfo *component) {
    auto boxColliderComponent =
        static_cast<BoxColliderComponent *>(component->instance);
    return boxColliderComponent->offset.y;
  });
  lua.set_function(
      "set_box_collider_position_y", [](ComponentInfo *component, int y) {
        auto boxColliderComponent =
            static_cast<BoxColliderComponent *>(component->instance);
        boxColliderComponent->offset.y = y;
      });

  // width
  lua.set_function("get_box_collider_width", [](ComponentInfo *component) {
    auto boxColliderComponent =
        static_cast<BoxColliderComponent *>(component->instance);
    return boxColliderComponent->dimensions.x;
  });
  lua.set_function(
      "set_box_collider_width", [](ComponentInfo *component, int width) {
        auto boxColliderComponent =
            static_cast<BoxColliderComponent *>(component->instance);
        boxColliderComponent->dimensions.x = width;
      });

  // height
  lua.set_function("get_box_collider_height", [](ComponentInfo *component) {
    auto boxColliderComponent =
        static_cast<BoxColliderComponent *>(component->instance);
    return boxColliderComponent->dimensions.y;
  });
  lua.set_function(
      "set_box_collider_height", [](ComponentInfo *component, int height) {
        auto boxColliderComponent =
            static_cast<BoxColliderComponent *>(component->instance);
        boxColliderComponent->dimensions.y = height;
      });
}
