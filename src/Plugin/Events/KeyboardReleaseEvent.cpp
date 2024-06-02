#include "KeyboardReleaseEvent.h"
#include <any>
#include <vector>

extern "C" void *createInstance(std::vector<std::any> args) {
  int keyCode = std::any_cast<int>(args[0]);
  return static_cast<void *>(new KeyReleaseEvent(keyCode));
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<KeyReleaseEvent *>(instance);
}

extern "C" const char *getEventName() { return "keyReleaseEvent"; }
