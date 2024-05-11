#include "SpriteComponent.h"

extern "C" void *createInstance(int width, int height, const char *id,
                                unsigned int zIndex, bool isFixed, int srcRectX,
                                int srcRectY) {
  PluginSpriteComponent *sprite = new PluginSpriteComponent(
      width, height, id, zIndex, isFixed, srcRectX, srcRectY);
  return static_cast<void *>(sprite);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginSpriteComponent *>(instance);
}
extern "C" const char *getComponentName() { return "SpriteComponent"; }
