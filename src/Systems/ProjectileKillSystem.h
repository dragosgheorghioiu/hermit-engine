#ifndef PROJECTILE_KILL_SYSTEM_H
#define PROJECTILE_KILL_SYSTEM_H

#include "../ECS/ECS.h"

class ProjectileKillSystem : public System {
public:
  ProjectileKillSystem();

  void Update();
};

#endif
