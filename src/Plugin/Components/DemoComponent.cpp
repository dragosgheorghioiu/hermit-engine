#include "DemoComponent.h"

extern "C" void *createInstance(int value, const char *str) {
  return static_cast<void *>(new DemoComponent(value, str));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoComponent *>(instance);
}
extern "C" const char *getComponentName() { return "DemoComponent"; }
