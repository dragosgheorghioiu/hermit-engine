#include "SpriteComponent.h"

extern "C" void *createInstance(int width, int height, const char *id,
                                unsigned int zIndex, bool isFixed, int srcRectX,
                                int srcRectY) {
  return static_cast<void *>(new SpriteComponent(width, height, id, zIndex,
                                                 isFixed, srcRectX, srcRectY));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<SpriteComponent *>(instance);
}
extern "C" const char *getComponentName() { return "SpriteComponent"; }
