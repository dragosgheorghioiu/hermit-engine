#include "TransformComponent.h"
#include "../../ECS/Entity.h"
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
  // transform position x
  lua.set_function("get_transform_position_x", [](EntityType *entity) {
    return static_cast<PluginTransformComponent *>(
               entity->getComponent("TransformComponent").instance)
        ->position.x;
  });
  lua.set_function("set_transform_position_x", [](EntityType *entity, float x) {
    static_cast<PluginTransformComponent *>(
        entity->getComponent("TransformComponent").instance)
        ->position.x = x;
  });

  // transform position y
  lua.set_function("get_transform_position_y", [](EntityType *entity) {
    return static_cast<PluginTransformComponent *>(
               entity->getComponent("TransformComponent").instance)
        ->position.y;
  });
  lua.set_function("set_transform_position_y", [](EntityType *entity, float y) {
    static_cast<PluginTransformComponent *>(
        entity->getComponent("TransformComponent").instance)
        ->position.y = y;
  });

  // transform scale x
  lua.set_function("get_transform_scale_x", [](EntityType *entity) {
    return static_cast<PluginTransformComponent *>(
               entity->getComponent("TransformComponent").instance)
        ->scale.x;
  });
  lua.set_function("set_transform_scale_x", [](EntityType *entity, float x) {
    static_cast<PluginTransformComponent *>(
        entity->getComponent("TransformComponent").instance)
        ->scale.x = x;
  });

  // transform scale y
  lua.set_function("get_transform_scale_y", [](EntityType *entity) {
    return static_cast<PluginTransformComponent *>(
               entity->getComponent("TransformComponent").instance)
        ->scale.y;
  });
  lua.set_function("set_transform_scale_y", [](EntityType *entity, float y) {
    static_cast<PluginTransformComponent *>(
        entity->getComponent("TransformComponent").instance)
        ->scale.y = y;
  });

  // transform rotation
  lua.set_function("get_transform_rotation", [](EntityType *entity) {
    return static_cast<PluginTransformComponent *>(
               entity->getComponent("TransformComponent").instance)
        ->rotation;
  });
  lua.set_function("set_transform_rotation", [](EntityType *entity, float r) {
    static_cast<PluginTransformComponent *>(
        entity->getComponent("TransformComponent").instance)
        ->rotation = r;
  });
}
