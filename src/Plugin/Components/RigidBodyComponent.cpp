#include "RigidBodyComponent.h"
#include "../PluginComponentFactory.h"
#include <sol/state.hpp>
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params

) {
  int velocityX = std::get<int>(component_params[0]);
  int velocityY = std::get<int>(component_params[1]);
  int accelerationX = std::get<int>(component_params[2]);
  int accelerationY = std::get<int>(component_params[3]);
  int maxVelocityX = std::get<int>(component_params[4]);
  int maxVelocityY = std::get<int>(component_params[5]);

  RigidBodyComponent *rigidBody = new RigidBodyComponent(
      glm::vec2(velocityX, velocityY), glm::vec2(accelerationX, accelerationY),
      glm::vec2(maxVelocityX, maxVelocityY));

  return static_cast<void *>(rigidBody);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<RigidBodyComponent *>(instance);
}

extern "C" const char *getComponentName() { return "RigidBodyComponent"; }

extern "C" void addLuaBindings(sol::state &lua) {
  // velocity x
  lua.set_function("get_rigidbody_velocity_x",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->velocity.x;
                   });
  lua.set_function("set_rigidbody_velocity_x",
                   [](ComponentInstance *component, float velocityX) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->velocity.x = velocityX;
                   });

  // velocity y
  lua.set_function("get_rigidbody_velocity_y",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->velocity.y;
                   });
  lua.set_function("set_rigidbody_velocity_y",
                   [](ComponentInstance *component, float velocityY) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->velocity.y = velocityY;
                   });

  // acceleration x
  lua.set_function("get_rigidbody_acceleration_x",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->acceleration.x;
                   });
  lua.set_function("set_rigidbody_acceleration_x",
                   [](ComponentInstance *component, float accelerationX) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->acceleration.x = accelerationX;
                   });

  // acceleration y
  lua.set_function("get_rigidbody_acceleration_y",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->acceleration.y;
                   });
  lua.set_function("set_rigidbody_acceleration_y",
                   [](ComponentInstance *component, float accelerationY) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->acceleration.y = accelerationY;
                   });

  // max velocity x
  lua.set_function("get_rigidbody_max_velocity_x",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->maxVelocity.x;
                   });

  lua.set_function("set_rigidbody_max_velocity_x",
                   [](ComponentInstance *component, float maxVelocityX) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->maxVelocity.x = maxVelocityX;
                   });

  // max velocity y
  lua.set_function("get_rigidbody_max_velocity_y",
                   [](ComponentInstance *component) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     return rigidBodyComponent->maxVelocity.y;
                   });
  lua.set_function("set_rigidbody_max_velocity_y",
                   [](ComponentInstance *component, float maxVelocityY) {
                     auto rigidBodyComponent =
                         static_cast<RigidBodyComponent *>(component->instance);
                     rigidBodyComponent->maxVelocity.y = maxVelocityY;
                   });
}
