#include "KeyboardPressEvent.h"
#include <any>
#include <vector>

extern "C" void *createInstance(std::vector<std::any> args) {
  int keyCode = std::any_cast<int>(args[0]);
  return static_cast<void *>(new KeyPressEvent(keyCode));
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<KeyPressEvent *>(instance);
}

extern "C" const char *getEventName() { return "keyPressEvent"; }
