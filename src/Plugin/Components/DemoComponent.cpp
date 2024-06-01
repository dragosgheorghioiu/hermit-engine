#include "DemoComponent.h"

extern "C" void *createInstance(int i) {
  return static_cast<void *>(new DemoComponent(i, "Hello World"));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoComponent *>(instance);
}
extern "C" const char *getComponentName() { return "PluginComponent"; }
