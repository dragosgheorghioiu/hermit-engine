#ifndef PROJECTILE_EMITTER_COMPONENT_H
#define PROJECTILE_EMITTER_COMPONENT_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  float velocity;
  int repeat;
  int duration;
  bool isFriendly;
  int damage;
  int lastTime;
  float angle;

  ProjectileEmitterComponent(float velocity = 0.0, int repeat = 0,
                             int duration = 10000, bool isFriendly = false,
                             int damage = 1, float angle = 0.0f)
      : velocity(velocity), repeat(repeat), duration(duration),
        isFriendly(isFriendly), damage(damage), lastTime(0), angle(angle) {}
};

#endif
