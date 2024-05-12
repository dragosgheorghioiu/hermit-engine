#include "RigidBodyComponent.h"

extern "C" void *createInstance(int velocityX, int velocityY, int accelerationX,
                                int accelerationY, int maxVelocityX,
                                int maxVelocityY) {
  RigidBodyComponent *rigidBody = new RigidBodyComponent(
      glm::vec2(velocityX, velocityY), glm::vec2(accelerationX, accelerationY),
      glm::vec2(maxVelocityX, maxVelocityY));
  return static_cast<void *>(rigidBody);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<RigidBodyComponent *>(instance);
}

extern "C" const char *getComponentName() { return "RigidBodyComponent"; }
