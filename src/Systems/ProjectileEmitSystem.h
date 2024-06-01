#ifndef PROJECTILE_EMIT_SYSTEM_H
#define PROJECTILE_EMIT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardPressEvent.h"
#include "fwd.hpp"

class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem();

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus);

  glm::vec2 SetProjectileAngle(const float &angle, const float &speed);

  void OnProjectileShoot(KeyPressEvent &e);

  void Update(std::unique_ptr<Registry> &registry);

  void CreateProjectile(Registry *registry, Entity &entity, glm::vec2 position,
                        glm::vec2 velocity, int duration, bool isFriendly,
                        int damage);
};

#endif
