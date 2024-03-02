#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#include <SDL2/SDL_timer.h>
struct ProjectileComponent {
  int duration;
  bool isFriendly;
  int damage;
  int startTime;

  ProjectileComponent(int duration = 10000, bool isFriendly = false,
                      int damage = 1)
      : duration(duration), isFriendly(isFriendly), damage(damage) {
    startTime = SDL_GetTicks();
  }
};

#endif
