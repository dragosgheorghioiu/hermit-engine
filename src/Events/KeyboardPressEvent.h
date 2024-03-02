#ifndef KEYBOARD_PRESS_EVENT_H
#define KEYBOARD_PRESS_EVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL_keycode.h>

class KeyPressEvent : public Event {
public:
  SDL_Keycode keyCode;

  KeyPressEvent(SDL_Keycode keyCode) : keyCode(keyCode) {}
};

#endif
