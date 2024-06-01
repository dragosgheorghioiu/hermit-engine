#include "KeyboardPressEvent.h"

extern "C" void *createInstance(SDL_Keycode keyCode) {
  return static_cast<void *>(new KeyPressEvent(keyCode));
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<KeyPressEvent *>(instance);
}

extern "C" const char *getEventName() { return "keyPressEvent"; }
