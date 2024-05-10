#include "TransformComponent.h"

extern "C" void *createInstance(glm::vec2 position, glm::vec2 scale,
                                double rotation) {
  PluginTransformComponent *transform =
      new PluginTransformComponent(position, scale, rotation);
  return static_cast<void *>(transform);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginTransformComponent *>(instance);
}
extern "C" const char *getComponentName() { return "TransformComponent"; }
