#ifndef KEYBOARD_RELEASE_EVENT_H
#define KEYBOARD_RELEASE_EVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL_keycode.h>

class KeyReleaseEvent : public Event {
public:
  SDL_Keycode keyCode;

  KeyReleaseEvent(SDL_Keycode keyCode) : keyCode(keyCode) {}
};

#endif
