#include "DemoEvent.h"

extern "C" void *createInstance(int i) {
  return static_cast<void *>(new DemoEvent(i, "Hello World"));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoEvent *>(instance);
}
extern "C" const char *getEventName() { return "PluginEvent"; }
