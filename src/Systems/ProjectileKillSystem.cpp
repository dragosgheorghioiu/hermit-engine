#include "ProjectileKillSystem.h"
#include "../Components/ProjectileComponent.h"

ProjectileKillSystem::ProjectileKillSystem() {
  RequireComponent<ProjectileComponent>();
}

void ProjectileKillSystem::Update() {
  for (auto entity : GetSystemEntities()) {
    ProjectileComponent &projectile =
        entity.GetComponent<ProjectileComponent>();
    if (SDL_GetTicks() - projectile.startTime >
        static_cast<Uint32>(projectile.duration)) {
      entity.Kill();
    }
  }
}
