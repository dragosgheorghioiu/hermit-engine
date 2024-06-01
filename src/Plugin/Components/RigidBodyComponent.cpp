#include "RigidBodyComponent.h"
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
