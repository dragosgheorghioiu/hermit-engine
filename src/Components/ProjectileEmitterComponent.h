#ifndef PROJECTILE_EMITTER_COMPONENT_H
#define PROJECTILE_EMITTER_COMPONENT_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  glm::vec2 velocity;
  int repeat;
  int duration;
  bool isFriendly;
  int damage;
  int lastTime;

  ProjectileEmitterComponent(glm::vec2 velocity = glm::vec2(0), int repeat = 0,
                             int duration = 10000, bool isFriendly = false,
                             int damage = 1)
      : velocity(velocity), repeat(repeat), duration(duration),
        isFriendly(isFriendly), damage(damage), lastTime(SDL_GetTicks()) {}
};

#endif
