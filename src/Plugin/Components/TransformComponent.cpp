#include "TransformComponent.h"

extern "C" void *createInstance(float x, float y, float rotation, float scale) {
  return static_cast<void *>(new TransformComponent(
      glm::vec2(x, y), glm::vec2(scale, scale), rotation));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<TransformComponent *>(instance);
}
extern "C" const char *getComponentName() { return "TransformComponent"; }
