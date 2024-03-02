#ifndef PROJECTILE_KILL_SYSTEM_H
#define PROJECTILE_KILL_SYSTEM_H

#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"

class ProjectileKillSystem : public System {
public:
  ProjectileKillSystem() { RequireComponent<ProjectileComponent>(); }

  void Update() {
    for (auto entity : GetSystemEntities()) {
      ProjectileComponent &projectile =
          entity.GetComponent<ProjectileComponent>();
      if (SDL_GetTicks() - projectile.startTime >
          static_cast<Uint32>(projectile.duration)) {
        entity.Kill();
      }
    }
  }
};

#endif
