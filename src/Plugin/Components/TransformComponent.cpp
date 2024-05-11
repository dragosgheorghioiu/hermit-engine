#include "TransformComponent.h"

extern "C" void *createInstance(int x, int y, int scaleX, int scaleY,
                                double rotation) {
  PluginTransformComponent *transform = new PluginTransformComponent(
      glm::vec2(x, y), glm::vec2(scaleX, scaleY), rotation);
  return static_cast<void *>(transform);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginTransformComponent *>(instance);
}
extern "C" const char *getComponentName() { return "TransformComponent"; }
