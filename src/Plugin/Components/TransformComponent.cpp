#include "TransformComponent.h"
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
extern "C" void addLuaBindings(sol::state &lua) {
  lua.set_function(
      "get_transform_position_x", [](ComponentInstance *component) {
        return static_cast<PluginTransformComponent *>(component->instance)
            ->position.x;
      });

  lua.set_function("set_transform_position_x", [](ComponentInstance *component,
                                                  float x) {
    static_cast<PluginTransformComponent *>(component->instance)->position.x =
        x;
  });

  lua.set_function(
      "get_transform_position_y", [](ComponentInstance *component) {
        return static_cast<PluginTransformComponent *>(component->instance)
            ->position.y;
      });
  lua.set_function("set_transform_position_y", [](ComponentInstance *component,
                                                  float y) {
    static_cast<PluginTransformComponent *>(component->instance)->position.y =
        y;
  });

  lua.set_function("get_transform_scale_x", [](ComponentInstance *component) {
    return static_cast<PluginTransformComponent *>(component->instance)
        ->scale.x;
  });
  lua.set_function("set_transform_scale_x", [](ComponentInstance *component,
                                               float x) {
    static_cast<PluginTransformComponent *>(component->instance)->scale.x = x;
  });

  lua.set_function("get_transform_scale_y", [](ComponentInstance *component) {
    return static_cast<PluginTransformComponent *>(component->instance)
        ->scale.y;
  });
  lua.set_function("set_transform_scale_y", [](ComponentInstance *component,
                                               float y) {
    static_cast<PluginTransformComponent *>(component->instance)->scale.y = y;
  });

  lua.set_function("get_transform_rotation", [](ComponentInstance *component) {
    return static_cast<PluginTransformComponent *>(component->instance)
        ->rotation;
  });
  lua.set_function("set_transform_rotation", [](ComponentInstance *component,
                                                float rotation) {
    static_cast<PluginTransformComponent *>(component->instance)->rotation =
        rotation;
  });
}
