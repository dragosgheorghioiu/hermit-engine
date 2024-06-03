#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <glm/vec2.hpp>

struct PlayerController {
  bool ignoreInput;

  PlayerController(bool ignoreInput = false) : ignoreInput(ignoreInput) {}
};

#endif
