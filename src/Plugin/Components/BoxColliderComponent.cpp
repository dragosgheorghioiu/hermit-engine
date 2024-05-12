#include "BoxColliderComponent.h"

extern "C" void *createInstance(int x, int y, int width, int height) {
  BoxColliderComponent *boxCollider =
      new BoxColliderComponent(glm::vec2(x, y), glm::vec2(width, height));
  return static_cast<void *>(boxCollider);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<BoxColliderComponent *>(instance);
}

extern "C" const char *getComponentName() { return "BoxColliderComponent"; }
