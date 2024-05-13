#ifndef KEYBOARDPRESSEVENT_H
#define KEYBOARDPRESSEVENT_H

#include <SDL2/SDL_keycode.h>

class KeyPressEvent {
public:
  SDL_Keycode keyCode;

  KeyPressEvent(SDL_Keycode keyCode) : keyCode(keyCode) {}
};

#endif
