#include "RigidBodyComponent.h"

extern "C" void *createInstance(int x, int y) {
  RigidBodyComponent *rigidBody = new RigidBodyComponent(glm::vec2(x, y));
  return static_cast<void *>(rigidBody);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<RigidBodyComponent *>(instance);
}

extern "C" const char *getComponentName() { return "RigidBodyComponent"; }
