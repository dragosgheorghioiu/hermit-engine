#include "KeyboardReleaseEvent.h"

extern "C" void *createInstance(SDL_Keycode keyCode) {
  return static_cast<void *>(new KeyReleaseEvent(keyCode));
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<KeyReleaseEvent *>(instance);
}

extern "C" const char *getEventName() { return "keyReleaseEvent"; }
